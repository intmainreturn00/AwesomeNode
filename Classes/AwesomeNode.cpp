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
    highlightTriangle(p1, p2, p3);
#endif

}


void AwesomeNode::drawALine(const Vec2 &A, const Vec2 &B, const Color4B &color, int w) {

    lineSegment s = calculateLineSegment(A, B, w);
    //drawLineSegment(s, color);
    drawASegment(A, B, w / 2, Color4F(color));
}


void AwesomeNode::drawACardinalSpline(PointArray *config, float tension, unsigned int segments,
                                      const Color4B &color, int w) {
    /// copy-pasted from DrawNode::drawCardinalSpline
    Vec2 *vertices = new(std::nothrow) Vec2[segments + 1];
    if (!vertices)
        return;

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

//    for (int i = 2; i <= segments; ++i) {
//        lineJoint curBorder = calculateLineJoint(vertices[i - 2], vertices[i - 1], vertices[i], w);
//
//        lineSegment segment = curBorder.segment;
//        segment.A1 = A1p;
//        segment.A2 = A2p;
//
//        Color4B border(color);
//        border.a = 0;
//
//        if (curBorder.up) {
//            drawTriangle(curBorder.segment.B, curBorder.segment.B2, curBorder.B4, color, border,
//                         border);
//            segment.B1 = curBorder.K;
//            drawLineSegment(segment, color);
//            A1p = curBorder.K;
//            A2p = curBorder.B4;
//        } else {
//            drawTriangle(curBorder.segment.B, curBorder.segment.B1, curBorder.B3, color, border,
//                         border);
//            segment.B2 = curBorder.K3;
//            drawLineSegment(segment, color);
//            A1p = curBorder.B3;
//            A2p = curBorder.K3;
//        }
//    }
//
//    lineSegment lastSegment = calculateLineSegment(vertices[segments - 1], vertices[segments], w);
//    lastSegment.A1 = A1p;
//    lastSegment.A2 = A2p;
//    drawLineSegment(lastSegment, color);

    for (int i = 2; i <= segments + 1; ++i) {
        drawASegment(vertices[i - 2], vertices[i - 1], w / 2, Color4F(color));
    }

    // draw above
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


static inline Vec2 v2f(float x, float y) {
    Vec2 ret(x, y);
    return ret;
}

static inline Vec2 v2fadd(const Vec2 &v0, const Vec2 &v1) {
    return v2f(v0.x + v1.x, v0.y + v1.y);
}

static inline Vec2 v2fsub(const Vec2 &v0, const Vec2 &v1) {
    return v2f(v0.x - v1.x, v0.y - v1.y);
}

static inline Vec2 v2fmult(const Vec2 &v, float s) {
    return v2f(v.x * s, v.y * s);
}

static inline Vec2 v2fperp(const Vec2 &p0) {
    return v2f(-p0.y, p0.x);
}

static inline Vec2 v2fneg(const Vec2 &p0) {
    return v2f(-p0.x, -p0.y);
}

static inline float v2fdot(const Vec2 &p0, const Vec2 &p1) {
    return p0.x * p1.x + p0.y * p1.y;
}

static inline Vec2 v2fnormalize(const Vec2 &p) {
    Vec2 r(p.x, p.y);
    r.normalize();
    return v2f(r.x, r.y);
}

static inline Vec2 __v2f(const Vec2 &v) {
//#ifdef __LP64__
    return v2f(v.x, v.y);
// #else
//     return * ((Vec2*) &v);
// #endif
}

static inline Tex2F __t(const Vec2 &v) {
    return *(Tex2F *) &v;
}


void
AwesomeNode::drawASegment(const Vec2 &from, const Vec2 &to, float radius, const Color4F &color) {
    unsigned int vertex_count = 6 * 3;
    ensureCapacity(vertex_count);

    Vec2 a = __v2f(from);
    Vec2 b = __v2f(to);


    Vec2 n = v2fnormalize(v2fperp(v2fsub(b, a)));
    Vec2 t = v2fperp(n);

    Vec2 nw = v2fmult(n, radius);
    Vec2 tw = v2fmult(t, radius);
    Vec2 v0 = v2fsub(b, v2fadd(nw, tw));
    Vec2 v1 = v2fadd(b, v2fsub(nw, tw));
    Vec2 v2 = v2fsub(b, nw);
    Vec2 v3 = v2fadd(b, nw);
    Vec2 v4 = v2fsub(a, nw);
    Vec2 v5 = v2fadd(a, nw);
    Vec2 v6 = v2fsub(a, v2fsub(nw, tw));
    Vec2 v7 = v2fadd(a, v2fadd(nw, tw));


    V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *) (_buffer + _bufferCount);

    V2F_C4B_T2F_Triangle triangles0 = {
            {v0, Color4B(color), __t(v2fneg(v2fadd(n, t)))},
            {v1, Color4B(color), __t(v2fsub(n, t))},
            {v2, Color4B(color), __t(v2fneg(n))},
    };
    triangles[0] = triangles0;

    V2F_C4B_T2F_Triangle triangles1 = {
            {v3, Color4B(color), __t(n)},
            {v1, Color4B(color), __t(v2fsub(n, t))},
            {v2, Color4B(color), __t(v2fneg(n))},
    };
    triangles[1] = triangles1;

    V2F_C4B_T2F_Triangle triangles2 = {
            {v3, Color4B(color), __t(n)},
            {v4, Color4B(color), __t(v2fneg(n))},
            {v2, Color4B(color), __t(v2fneg(n))},
    };
    triangles[2] = triangles2;

    V2F_C4B_T2F_Triangle triangles3 = {
            {v3, Color4B(color), __t(n)},
            {v4, Color4B(color), __t(v2fneg(n))},
            {v5, Color4B(color), __t(n)},
    };
    triangles[3] = triangles3;

    V2F_C4B_T2F_Triangle triangles4 = {
            {v6, Color4B(color), __t(v2fsub(t, n))},
            {v4, Color4B(color), __t(v2fneg(n))},
            {v5, Color4B(color), __t(n)},
    };
    triangles[4] = triangles4;

    V2F_C4B_T2F_Triangle triangles5 = {
            {v6, Color4B(color), __t(v2fsub(t, n))},
            {v7, Color4B(color), __t(v2fadd(n, t))},
            {v5, Color4B(color), __t(n)},
    };
    triangles[5] = triangles5;

    _bufferCount += vertex_count;

    _dirty = true;

#ifdef AWESOMEDEBUG
    highlightTriangle(v0, v1, v2);
    highlightTriangle(v3, v1, v2);
    highlightTriangle(v3, v4, v2);
    highlightTriangle(v3, v4, v5);
    highlightTriangle(v6, v4, v5);
    highlightTriangle(v6, v7, v5);

#endif

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

void AwesomeNode::highlightTriangle(const Vec2 &A, const Vec2 &B, const Vec2 &C) {
    drawLine(A, B, Color4F::GREEN);
    drawLine(B, C, Color4F::GREEN);
    drawLine(C, A, Color4F::GREEN);
}

V2F_C4B_T2F __tct(float x, float y, Color4B color, float texx, float texy) {
    return V2F_C4B_T2F{Vec2(x, y), color, Tex2F(texx, texy)};
}

void AwesomeNode::testTrianglesTex() {

    Color4B color(Color4B::RED);

    unsigned int vertex_count = 3 * 4;
    ensureCapacity(vertex_count);

    V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *) (_buffer + _bufferCount);

    triangles[0] = V2F_C4B_T2F_Triangle {__tct(300, 150, color, 0, 1),
                                         __tct(350, 150, color, 1, 1),
                                         __tct(300, 50, color, 0, -1)};

    triangles[1] = V2F_C4B_T2F_Triangle {__tct(300, 50, color, 0, -1),
                                         __tct(350, 150, color, 1, 1),
                                         __tct(350, 50, color, 1, -1)};

    triangles[2] = V2F_C4B_T2F_Triangle {__tct(300, 150, color, 0, 1),
                                         __tct(250, 50, color, -1, -1),
                                         __tct(300, 50, color, 0, -1)};

    triangles[3] = V2F_C4B_T2F_Triangle {__tct(300, 150, color, 0, 1),
                                         __tct(250, 150, color, -1, 1),
                                         __tct(250, 50, color, -1, -1)};

    _bufferCount += vertex_count;
    _dirty = true;

    //const Vec2 &center, float radius, float angle, unsigned int segments, bool drawLineToCenter, const Color4F &color);
    drawCircle(Vec2(50, 100), 50, 360, 20, false, Color4F::RED);
    drawDot(Vec2(150, 100), 50, Color4F::RED);


    drawASegment(Vec2(250, 250), Vec2(300, 200), 20, Color4F::RED);
    drawASegment(Vec2(300, 200), Vec2(350, 250), 20, Color4F::BLUE);


    lineSegment seg = calculateLineSegment(Vec2(400, 200), Vec2(450, 250), 2);

    DrawNode::drawTriangle(seg.A1, seg.B1, seg.A2, Color4F::RED);
    DrawNode::drawTriangle(seg.A2, seg.B1, seg.B2, Color4F::RED);

    //drawTriangle(Vec2(), Vec2(), Vec2(), Color4F::RED);


#ifdef AWESOMEDEBUG
//    highlightTriangle(a.vertices, b.vertices, c.vertices);
//    highlightTriangle(a1.vertices, b1.vertices, c1.vertices);
#endif

}

