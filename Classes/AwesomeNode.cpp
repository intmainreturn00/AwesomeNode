#include "AwesomeNode.h"

const int AwesomeNode::LINE_SIZE_THRESHOLD = 4;

USING_NS_CC;

bool AwesomeNode::init() {
    return DrawNode::init();
}


void AwesomeNode::drawTriangle(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3,
                               const Color4F &color1, const Color4F &color2,
                               const Color4F &color3) {

    unsigned int vertex_count = 3;
    ensureCapacity(vertex_count);

    V2F_C4B_T2F a = {Vec2(p1.x, p1.y), Color4B(color1), Tex2F(0.0, 0.0)};
    V2F_C4B_T2F b = {Vec2(p2.x, p2.y), Color4B(color2), Tex2F(0.0, 0.0)};
    V2F_C4B_T2F c = {Vec2(p3.x, p3.y), Color4B(color3), Tex2F(0.0, 0.0)};

    auto *triangles = (V2F_C4B_T2F_Triangle *) (_buffer + _bufferCount);
    V2F_C4B_T2F_Triangle triangle = {a, b, c};
    triangles[0] = triangle;

    _bufferCount += vertex_count;
    _dirty = true;

#ifdef AWESOMEDEBUG
    highlightTriangle(p1, p2, p3);
#endif

}


void AwesomeNode::drawALine(const Vec2 &A, const Vec2 &B, float w, const Color4F &color) {

    if (w < LINE_SIZE_THRESHOLD) {
        lineSegment s = calculateLineSegment(A, B, w);
        drawLineSegment(s, color);
    } else {
        drawSegment(A, B, w / 2, color);
    }
}


void AwesomeNode::drawACardinalSpline(PointArray *config, float tension, unsigned int segments,
                                      float w, const Color4F &color) {

    auto *vertices = calculateVertices(config, tension, segments);

    if (w < LINE_SIZE_THRESHOLD) {
        tessellation(vertices, segments, w, color);
    } else {
        for (int i = 2; i <= segments + 1; ++i) {
            drawSegment(vertices[i - 2], vertices[i - 1], w / 2, color);
        }
    }

#ifdef AWESOMEDEBUG
    for (int i = 0; i < segments + 1; ++i) {
        drawDot(vertices[i], 2, Color4F::BLUE);
    }
    for (int i = 0; i < config->count(); ++i) {
        drawDot(config->getControlPointAtIndex(i), 3, Color4F::GREEN);
    }
#endif

    CC_SAFE_DELETE_ARRAY(vertices);
}


void AwesomeNode::drawLineSegment(const AwesomeNode::lineSegment &segment, const Color4F &color) {
    Color4F border(color);
    border.a = 0;

    drawTriangle(segment.A1, segment.B1, segment.A, border, border, color);
    drawTriangle(segment.A, segment.B1, segment.B, color, border, color);
    drawTriangle(segment.A, segment.B, segment.A2, color, color, border);
    drawTriangle(segment.A2, segment.B, segment.B2, border, color, border);
}


AwesomeNode::lineJoint
AwesomeNode::calculateLineJoint(const Vec2 &A, const Vec2 &B, const Vec2 &C, float w) {
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


AwesomeNode::lineSegment AwesomeNode::calculateLineSegment(const Vec2 &A, const Vec2 &B, float w) {
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

void AwesomeNode::highlightTriangle(const Vec2 &A, const Vec2 &B, const Vec2 &C) {
    drawLine(A, B, Color4F::GREEN);
    drawLine(B, C, Color4F::GREEN);
    drawLine(C, A, Color4F::GREEN);
}

void
AwesomeNode::drawADashDottedLine(const Vec2 &from, const Vec2 &to, float w, float dashSize,
                                 const Color4F &color) {
    auto steps = (int) (from.distance(to) / (dashSize * 1.1));
    Vec2 v(to.x - from.x, to.y - from.y);
    v.normalize();
    Vec2 A, B, C;
    A.x = from.x;
    A.y = from.y;
    for (int i = 0; i < steps; ++i) {
        B.x = A.x + v.x * dashSize * 0.7f;
        B.y = A.y + v.y * dashSize * 0.7f;
        C.x = B.x + v.x * (dashSize * 0.2f);
        C.y = B.y + v.y * (dashSize * 0.2f);

        drawALine(A, B, w, color);
        drawDot(C, w / 2, color);

        A.x = C.x + v.x * (dashSize * 0.2f);
        A.y = C.y + v.y * (dashSize * 0.2f);
    }

    drawALine(A, to, w, color);
}

void AwesomeNode::drawADashedLine(const Vec2 &from, const Vec2 &to, float w, float dashSize,
                                  const Color4F &color) {
    auto steps = (int) (from.distance(to) / (dashSize * 1.3));
    Vec2 v(to.x - from.x, to.y - from.y);
    v.normalize();
    Vec2 A, B;
    A.x = from.x;
    A.y = from.y;
    for (int i = 0; i < steps; ++i) {
        B.x = A.x + v.x * dashSize;
        B.y = A.y + v.y * dashSize;

        drawALine(A, B, w, color);

        A.x = B.x + v.x * (dashSize * 0.3f);
        A.y = B.y + v.y * (dashSize * 0.3f);
    }

    drawALine(A, to, w, color);
}

void
AwesomeNode::drawAFilledCardinalSpline(PointArray *config, float tension, unsigned int segments,
                                       float w, const Color4F &color, float bottomY,
                                       const Color4F &fill) {
    auto *vertices = calculateVertices(config, tension, segments);

    if (w < LINE_SIZE_THRESHOLD) {

        for (int i = 2; i <= segments; ++i) {
            lineJoint cur = calculateLineJoint(vertices[i - 2], vertices[i - 1], vertices[i], w);
            drawSolidRect(cur.segment.A, cur.segment.B,
                          Vec2(cur.segment.B.x, bottomY),
                          Vec2(cur.segment.A.x, bottomY), fill);
        }

        lineSegment last = calculateLineSegment(vertices[segments - 1], vertices[segments], w);
        drawSolidRect(last.A, last.B, Vec2(last.B.x, bottomY), Vec2(last.A.x, bottomY), fill);

        tessellation(vertices, segments, w, color);

    } else {

        for (int i = 2; i <= segments + 1; ++i) {
            drawSolidRect(Vec2(vertices[i - 2].x, vertices[i - 2].y),
                          Vec2(vertices[i - 1].x, vertices[i - 1].y),
                          Vec2(vertices[i - 1].x, bottomY),
                          Vec2(vertices[i - 2].x, bottomY), fill);
        }
        for (int i = 2; i <= segments + 1; ++i) {
            drawSegment(vertices[i - 2], vertices[i - 1], w / 2, color);
        }
    }

#ifdef AWESOMEDEBUG
    for (int i = 0; i < segments + 1; ++i) {
        drawDot(vertices[i], 2, Color4F::BLUE);
    }
    for (int i = 0; i < config->count(); ++i) {
        drawDot(config->getControlPointAtIndex(i), 3, Color4F::GREEN);
    }
#endif

    CC_SAFE_DELETE_ARRAY(vertices);
}

void AwesomeNode::drawSolidRect(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3, const Vec2 &p4,
                                const Color4F &color) {
    Vec2 vertices[] = {p1, p2, p3, p4};
    drawSolidPoly(vertices, 4, color);
}

void AwesomeNode::drawSolidRect(const Vec2 &origin, const Vec2 &destination, const Color4F &color) {
    DrawNode::drawSolidRect(origin, destination, color);
}

Vec2 *AwesomeNode::calculateVertices(PointArray *config, float tension, unsigned int segments) {
    auto *vertices = new Vec2[segments + 1];
    // copied from DrawNode cardinal splines with fix for border
    ssize_t p;
    float lt;
    float deltaT = 1.0f / (config->count() - 1);

    for (unsigned int i = 0; i < segments + 1; i++) {
        float dt = (float) i / segments;
        // border
        if (dt == 1) {
            p = config->count() - 1;
            lt = 1;
        } else {
            p = (ssize_t) (dt / deltaT);
            lt = (dt - deltaT * (float) p) / deltaT;
        }
        // Interpolate
        const Vec2 &pp0 = config->getControlPointAtIndex(p - 1);
        const Vec2 &pp1 = config->getControlPointAtIndex(p + 0);
        const Vec2 &pp2 = config->getControlPointAtIndex(p + 1);
        const Vec2 &pp3 = config->getControlPointAtIndex(p + 2);

        Vec2 newPos = ccCardinalSplineAt(pp0, pp1, pp2, pp3, tension, lt);
        vertices[i].x = newPos.x;
        vertices[i].y = newPos.y;
    }

    return vertices;
}

void
AwesomeNode::tessellation(Vec2 *vertices, unsigned int segments, float w, const Color4F &color) {
    Color4F border(color);
    border.a = 0;

    Vec2 A1p, A2p;
    {
        lineSegment prev = calculateLineSegment(vertices[0], vertices[1], w);
        A1p = prev.A1;
        A2p = prev.A2;
    }

    for (int i = 2; i <= segments; ++i) {
        lineJoint cur = calculateLineJoint(vertices[i - 2], vertices[i - 1], vertices[i], w);

        lineSegment segment = cur.segment;
        segment.A1 = A1p;
        segment.A2 = A2p;

        if (cur.up) {
            drawTriangle(cur.segment.B, cur.segment.B2, cur.B4, color, border, border);
            segment.B1 = cur.K;
            drawLineSegment(segment, color);
            A1p = cur.K;
            A2p = cur.B4;
        } else {
            drawTriangle(cur.segment.B, cur.segment.B1, cur.B3, color, border, border);
            segment.B2 = cur.K3;
            drawLineSegment(segment, color);
            A1p = cur.B3;
            A2p = cur.K3;
        }
    }

    lineSegment last = calculateLineSegment(vertices[segments - 1], vertices[segments], w);
    last.A1 = A1p;
    last.A2 = A2p;

    drawLineSegment(last, color);
}
