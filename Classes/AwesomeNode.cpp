#include "AwesomeNode.h"


bool AwesomeNode::init() {
    return DrawNode::init();
}


void AwesomeNode::setOpacity(GLubyte opacity) {
    this->opacity = opacity;
    _displayedOpacity = _realOpacity = opacity;
    if (_bufferCount) {
        for (int i = 0; i < _bufferCount; i++) {
            _buffer[i].colors.a = opacity;
        }
    }

    if (_bufferCountGLPoint) {
        for (int i = 0; i < _bufferCountGLPoint; i++) {
            _bufferGLPoint[i].colors.a = opacity;
        }
    }

    if (_bufferCountGLLine) {
        for (int i = 0; i < _bufferCountGLLine; i++) {
            _bufferGLLine[i].colors.a = opacity;
        }
        _dirtyGLLine = true;
    }
    _dirty = true;
}


void AwesomeNode::drawTriangle(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3,
                               const Color4B &color1, const Color4B &color2,
                               const Color4B &color3) {

    unsigned int vertex_count = 3;
    ensureCapacity(vertex_count);

    V2F_C4B_T2F a = {Vec2(p1.x, p1.y), color1, Tex2F(0.0, 0.0)};
    V2F_C4B_T2F b = {Vec2(p2.x, p2.y), color2, Tex2F(0.0, 0.0)};
    V2F_C4B_T2F c = {Vec2(p3.x, p3.y), color3, Tex2F(0.0, 0.0)};

    V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *) (_buffer + _bufferCount);
    V2F_C4B_T2F_Triangle triangle = {a, b, c};
    triangles[0] = triangle;

    _bufferCount += vertex_count;
    _dirty = true;

#ifdef AWESOMEDEBUG
    drawLine(p1, p2, Color4F::MAGENTA);
    drawLine(p2, p3, Color4F::MAGENTA);
    drawLine(p3, p1, Color4F::MAGENTA);
#endif // AWESOMEDEBUG

}


void AwesomeNode::drawALine(const Vec2 &A, const Vec2 &B, const Color4B &color, int w) {

    lineSegment s = calculateLineSegment(A, B, w);
    drawLineSegment(s, color);
}


void AwesomeNode::drawACardinalSpline(PointArray *config, float tension, unsigned int segments,
                                      const Color4B &color, int w) {
    /// copy-pasted from DrawNode::drawCardinalSpline
    Vec2 *vertices = new(std::nothrow) Vec2[segments + 1];
    if (!vertices)
        return;

    ssize_t p;
    float lt;
    float deltaT = 1.0f / config->count();

    for (unsigned int i = 0; i < segments + 1; i++) {

        float dt = (float) i / segments;

        // border
        if (dt == 1) {
            p = config->count() - 1;
            lt = 1;
        } else {
            p = dt / deltaT;
            lt = (dt - deltaT * (float) p) / deltaT;
        }

        // Interpolate
        Vec2 pp0 = config->getControlPointAtIndex(p - 1);
        Vec2 pp1 = config->getControlPointAtIndex(p + 0);
        Vec2 pp2 = config->getControlPointAtIndex(p + 1);
        Vec2 pp3 = config->getControlPointAtIndex(p + 2);

        Vec2 newPos = ccCardinalSplineAt(pp0, pp1, pp2, pp3, tension, lt);
        vertices[i].x = newPos.x;
        vertices[i].y = newPos.y;
    }

    Vec2 A1p, A2p;
    {
        lineSegment prevBorder = calculateLineSegment(vertices[0], vertices[1], w);
        A1p = prevBorder.A1;
        A2p = prevBorder.A2;
    }

    for (int i = 2; i < segments; ++i) {
        lineJoint curBorder = calculateLineJoint(vertices[i - 2], vertices[i - 1], vertices[i], w);

        lineSegment segment = curBorder.segment;
        segment.A1 = A1p;
        segment.A2 = A2p;

        Color4B border(color);
        border.a = 0;

        if (curBorder.up) {
            drawTriangle(curBorder.segment.B, curBorder.segment.B2, curBorder.B4, color, border,
                         border);
            segment.B1 = curBorder.K;
            drawLineSegment(segment, color);
            A1p = curBorder.K;
            A2p = curBorder.B4;
        } else {
            drawTriangle(curBorder.segment.B, curBorder.segment.B1, curBorder.B3, color, border,
                         border);
            segment.B2 = curBorder.K3;
            drawLineSegment(segment, color);
            A1p = curBorder.B3;
            A2p = curBorder.K3;
        }
    }

    lineSegment lastSegment = calculateLineSegment(vertices[segments - 2], vertices[segments - 1], w);
    lastSegment.A1 = A1p;
    lastSegment.A2 = A2p;
    drawLineSegment(lastSegment, color);

    CC_SAFE_DELETE_ARRAY(vertices);
}


void AwesomeNode::drawLineSegment(const AwesomeNode::lineSegment &segment, const Color4B &color) {
    Color4B border(color);
    border.a = 0;

    drawTriangle(segment.A1, segment.B1, segment.A, border, border, color);
    drawTriangle(segment.A, segment.B1, segment.B, color, border, color);
    drawTriangle(segment.A, segment.B, segment.A2, color, color, border);
    drawTriangle(segment.A2, segment.B, segment.B2, border, color, border);
}


AwesomeNode::lineJoint
AwesomeNode::calculateLineJoint(const Vec2 &A, const Vec2 &B, const Vec2 &C, int w) {
    lineJoint res;
    w = w / 2;
    Vec2 n1(A.y - B.y, B.x - A.x), n2(B.y - A.y, A.x - B.x);
    Vec2 n3(B.y - C.y, C.x - B.x), n4(C.y - B.y, B.x - C.x);

    n1.normalize();
    n2.normalize();
    n3.normalize();
    n4.normalize();

    res.segment.A = A;
    res.segment.B = B;
    res.segment.A1 = Vec2(A.x + n1.x * w, A.y + n1.y * w);
    res.segment.A2 = Vec2(A.x + n2.x * w, A.y + n2.y * w);
    res.segment.B1 = Vec2(B.x + n1.x * w, B.y + n1.y * w);
    res.segment.B2 = Vec2(B.x + n2.x * w, B.y + n2.y * w);
    res.B3 = Vec2(B.x + n3.x * w, B.y + n3.y * w);
    res.B4 = Vec2(B.x + n4.x * w, B.y + n4.y * w);

    float angle1 = atan2(n1.x * n3.y - n1.y * n3.x, n1.x * n3.x + n1.y * n3.y);
    res.up = angle1 > 0;

    Vec2 b13((n1.x + n3.x), (n1.y + n3.y));
    b13.normalize();
    res.K = Vec2(B.x + b13.x * (w), B.y + b13.y * (w));

    Vec2 b24((n2.x + n4.x), (n2.y + n4.y));
    b24.normalize();
    res.K3 = Vec2(B.x + b24.x * (w), B.y + b24.y * (w));

    return res;
}


AwesomeNode::lineSegment AwesomeNode::calculateLineSegment(const Vec2 &A, const Vec2 &B, int w) {
    lineSegment res;

    Vec2 p(B.x - A.x, B.y - A.y);
    p.normalize();
    Vec2 p1(-p.y, p.x), p2(p.y, -p.x);

    res.A = A;
    res.B = B;
    res.A1 = Vec2(A + p1 * w / 2);
    res.A2 = Vec2(A + p2 * w / 2);
    res.B1 = Vec2(B + p1 * w / 2);
    res.B2 = Vec2(B + p2 * w / 2);

    return res;
}

