/*
* AwesomeNode.h
*
*  Created on: October 1, 2018
*      Author: Andrey Suvorov
*/

#pragma once

#include "cocos2d.h"

USING_NS_CC;


class AwesomeNode : public DrawNode {

public:

    void setOpacity(GLubyte opacity);

    AwesomeNode(int width, int borderWidth) : W(width), w(borderWidth),
                                              DrawNode() { /*_blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;*/ }

    int W, w;

    static AwesomeNode *create(int width = 5, int borderWidth = 2) {
        AwesomeNode *ret = new(std::nothrow) AwesomeNode(width, borderWidth);
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        } else {
            delete ret;
            return nullptr;
        }
    }

    virtual bool init();

    void drawTriangle(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3,
                      const Color4B &color1, const Color4B &color2, const Color4B &color3);

    void drawCardinalSpline(PointArray *config, float tension, unsigned int segments,
                            const Color4F &color) {
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

        Color4B border(color);
        border.a = 0;
        Color4B main(color);

        Color4B test(Color4B::MAGENTA);


        prevUPStatus = -1;

        for (unsigned int i = 2; i < segments; ++i) {
            Vec2 A(vertices[i - 2]);
            Vec2 B(vertices[i - 1]);
            Vec2 C(vertices[i]);

//            drawPoint(A, 4, Color4F::MAGENTA);
//            drawPoint(B, 4, Color4F::MAGENTA);
//            drawPoint(C, 4, Color4F::MAGENTA);

            drawLineSegment(A, B, C, W, w, main, border);

        }
        // last segment without C. need to approximate
        if (prevUPStatus != -1) {
            Vec2 A(vertices[segments - 2]);
            Vec2 B(vertices[segments - 1]);
            Vec2 C(B);

            drawLineSegment(A, B, C, W, w, main, border);
        }


        //drawPoly(vertices, segments + 1, false, color);
//        Color4F border = color;
//        border.a = 50;
//        drawPolygon(vertices, segments + 1, color, 1, border);

        CC_SAFE_DELETE_ARRAY(vertices);
    }

    // -1 not started, 0 - up-convex, 1 - up-inner
    int prevUPStatus = -1;
    Vec2 GK, GK2, GK3, GK4;

    void drawLineSegment(Vec2 &A, Vec2 &B, Vec2 &C, int W, int w, Color4B main, Color4B border) {
        Vec2 n1(A.y - B.y, B.x - A.x), n2(B.y - A.y, A.x - B.x);
        Vec2 n3(B.y - C.y, C.x - B.x), n4(C.y - B.y, B.x - C.x);

        n1.normalize();
        n2.normalize();
        n3.normalize();
        n4.normalize();

        Vec2 A1(A.x + n1.x * W, A.y + n1.y * W),
                A2(A.x + n2.x * W, A.y + n2.y * W);
        Vec2 B1(B.x + n1.x * W, B.y + n1.y * W),
                B2(B.x + n2.x * W, B.y + n2.y * W),
                B3(B.x + n3.x * W, B.y + n3.y * W),
                B4(B.x + n4.x * W, B.y + n4.y * W);
        Vec2 C1(C.x + n3.x * W, C.y + n3.y * W),
                C2(C.x + n4.x * W, C.y + n4.y * W);


        Vec2 Ab1(A.x + n1.x * (W + w), A.y + n1.y * (W + w)),
                Ab2(A.x + n2.x * (W + w), A.y + n2.y * (W + w));
        Vec2 Bb1(B.x + n1.x * (W + w), B.y + n1.y * (W + w)),
                Bb2(B.x + n2.x * (W + w), B.y + n2.y * (W + w)),
                Bb3(B.x + n3.x * (W + w), B.y + n3.y * (W + w)),
                Bb4(B.x + n4.x * (W + w), B.y + n4.y * (W + w));
        Vec2 Cb1(C.x + n3.x * (W + w), C.y + n3.y * (W + w)),
                Cb2(C.x + n4.x * (W + w), C.y + n4.y * (W + w));

        //angle = atan2( a.x*b.y - a.y*b.x, a.x*b.x + a.y*b.y );
        float angle1 = atan2(n1.x * n3.y - n1.y * n3.x, n1.x * n3.x + n1.y * n3.y);
        float angle2 = atan2(n2.x * n4.y - n2.y * n4.x, n2.x * n4.x + n2.y * n4.y);

        // TESSELATION //
        /////////////////

        // joint triangles

        if (angle1 > 0) {
            // up case - inner. need to add join triangle to convex (down)
            drawTriangle(B4, B2, B, Color4B::BLUE, Color4B::BLUE, Color4B::BLUE);
//            drawTriangle(B4, B2, B, main, main, main);

        } else {
            // down case - inner. need to add join triangle to convex (up)
            drawTriangle(B1, B3, B, Color4B::BLUE, Color4B::BLUE, Color4B::BLUE);
//            drawTriangle(B1, B3, B, main, main, main);

        }

        // Poly triangles
        drawTriangle(A1, B1, A2, main, main, main);
        drawTriangle(B1, A2, B2, main, main, main);


        // ANTI-ALIASING //
        ///////////////////

        // joint triangles

        Color4B test(Color4B::MAGENTA);
        Color4B testA(Color4B::MAGENTA);
        testA.a = 0;

        if (angle1 > 0) {
            // up case - inner.
            drawTriangle(Bb4, B4, B2, testA, test, test);
            drawTriangle(Bb4, B2, Bb2, testA, test, testA);
//            drawTriangle(Bb4, B4, B2, border, main, main);
//            drawTriangle(Bb4, B2, Bb2, border, main, border);
        } else {
            drawTriangle(B1, Bb1, Bb3, test, testA, testA);
            drawTriangle(B1, Bb3, B3, test, testA, test);
//            drawTriangle(B1, Bb1, Bb3, main, border, border);
//            drawTriangle(B1, Bb3, B3, main, border, main);
        }


        Color4B test2(Color4B::GREEN);
        Color4B test2A(Color4B::GREEN);
        test2A.a = 0;

        Color4B test3(Color4B::ORANGE);
        Color4B test3A(Color4B::ORANGE);
        test3A.a = 0;

        // Poly triangles
        /*drawTriangle(Ab1, A1, Bb1, border, main, border);
        drawTriangle(A1, Bb1, B1, main, border, main);*/

        drawTriangle(Ab1, A1, Bb1, test2A, test2, test2A);
        drawTriangle(A1, Bb1, B1, test2, test2A, test2);


        Vec2 b13((n1.x + n3.x), (n1.y + n3.y));
        b13.normalize();

        Vec2 K(B.x + b13.x * (W), B.y + b13.y * (W)),
                K2(B.x + b13.x * (W + w), B.y + b13.y * (W + w));
        Vec2 b24((n2.x + n4.x), (n2.y + n4.y));
        b24.normalize();

        Vec2 K3(B.x + b24.x * (W), B.y + b24.y * (W)),
                K4(B.x + b24.x * (W + w), B.y + b24.y * (W + w));

        drawPoint(K, 4, Color4F::BLUE);
        drawPoint(K2, 4, Color4F::BLUE);
        drawPoint(K3, 4, Color4F::BLUE);
        drawPoint(K4, 4, Color4F::BLUE);

        if (angle1 > 0) {
            // up - inner.

            if (prevUPStatus == -1) {
                drawTriangle(Ab1, A1, K2, test2A, test2, test2A);
                drawTriangle(A1, K2, K, test2, test2A, test2);
//                drawTriangle(Ab1, A1, K2, border, main, border);
//                drawTriangle(A1, K2, K, main, border, main);
            } else {
                drawTriangle(GK2, GK, K2, test2A, test2, test2A);
                drawTriangle(GK, K2, K, test2, test2A, test2);
//                drawTriangle(GK2, GK, K2, border, main, border);
//                drawTriangle(GK, K2, K, main, border, main);
            }


            //down - convex
            if (prevUPStatus == -1) {
                drawTriangle(Ab2, A2, B2, test2A, test2, test2);
                drawTriangle(Ab2, B2, Bb2, test2A, test2, test2A);
//                drawTriangle(Ab2, A2, B2, border, main, main);
//                drawTriangle(Ab2, B2, Bb2, border, main, border);
            } else {
                drawTriangle(GK4, GK3, B2, test2A, test2, test2);
                drawTriangle(GK4, B2, Bb2, test2A, test2, test2A);
//                drawTriangle(GK4, GK3, B2, border, main, main);
//                drawTriangle(GK4, B2, Bb2, border, main, border);
            }


            // -1 not started, 0 - up-convex, 1 - up-inner
            prevUPStatus = 1;
            GK = K;
            GK2 = K2;
            GK3 = B4;
            GK4 = Bb4;

        } else {
            // up - convex
            if (prevUPStatus == -1) {
                drawTriangle(Ab1, A1, Bb1, test2A, test2, test2A);
                drawTriangle(A1, Bb1, B1, test2, test2A, test2);
//                drawTriangle(Ab1, A1, Bb1, border, main, border);
//                drawTriangle(A1, Bb1, B1, main, border, main);
            } else {
                drawTriangle(GK2, GK, Bb1, border, main, border);
                drawTriangle(GK, Bb1, B1, main, border, main);
            }

            // down - inner.
            //drawTriangle(Ab2, A2, K3, test3A, test3, test3);
            //drawTriangle(Ab2, K3, K4, test2A, test2, test2A);
            if (prevUPStatus == -1) {
                drawTriangle(Ab2, A2, K3, test3A, test3, test3);
                drawTriangle(Ab2, K3, K4, test2A, test2, test2A);
//                drawTriangle(Ab2, A2, K3, border, main, main);
//                drawTriangle(Ab2, K3, K4, border, main, border);
            } else {
                drawTriangle(GK4, GK3, K3, test3A, test3, test3);
                drawTriangle(GK4, K3, K4, test2A, test2, test2A);
//                drawTriangle(GK4, GK3, K3, border, main, main);
//                drawTriangle(GK4, K3, K4, border, main, border);
            }


            // -1 not started, 0 - up-convex, 1 - up-inner
            prevUPStatus = 0;
            GK = B3;
            GK2 = Bb3;
            GK3 = K3;
            GK4 = K4;
        }






        /*drawTriangle(Ab2, A2, B2, border, main, main);
        drawTriangle(Ab2, B2, Bb2, border, main, border);*/

        drawTriangle(Ab2, A2, B2, test3A, test3, test3);
        drawTriangle(Ab2, B2, Bb2, test2A, test2, test2A);



    }


    /*void tesselation(Vec2 &B1, Vec2 &B2, Vec2 &I1, Vec2 &I2, Color4B &border, Color4B &inner) {
        drawTriangle(I1, B1, B2, inner, border, border);
        drawTriangle(I1, B2, I2, inner, border, inner);
    }*/



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

    static inline Vec2 v2fforangle(float _a_) {
        return v2f(cosf(_a_), sinf(_a_));
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


private:
    GLubyte opacity;
};

