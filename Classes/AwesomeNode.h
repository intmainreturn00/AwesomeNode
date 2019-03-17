/*
* AwesomeNode.h
*
*  Created on: October 1, 2018
*      Author: Andrey Suvorov
*/

#pragma once

#include "cocos2d.h"

USING_NS_CC;

//#define AWESOMEDEBUG

class AwesomeNode : public DrawNode {

public:

    void setOpacity(GLubyte opacity) override;

    CREATE_FUNC(AwesomeNode);

    bool init() override;

    /**draw triangle when each vertex can has its own color.*/
    void drawTriangle(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3,
                      const Color4B &color1, const Color4B &color2, const Color4B &color3);

    /** draw anti-aliased line */
    void drawALine(const Vec2 &A, const Vec2 &B, const Color4B &color, int w);

    void drawACardinalSpline(PointArray *config, float tension, unsigned int segments,
                             const Color4B &color, int w);

    void drawASegment(const Vec2 &from, const Vec2 &to, float radius, const Color4F &color);

    void testTrianglesTex();

private:

    GLubyte opacity;

    struct lineSegment {
        Vec2 A, A1, A2;
        Vec2 B, B1, B2;
    };

    struct lineJoint {
        lineSegment segment;
        Vec2 B3, B4;
        Vec2 K, K3;
        bool up;
    };

    void drawLineSegment(const lineSegment &segment, const Color4B &color);

    lineJoint calculateLineJoint(const Vec2 &A, const Vec2 &B, const Vec2 &C, int w);

    lineSegment calculateLineSegment(const Vec2 &A, const Vec2 &B, int w);

    void highlightTriangle(const Vec2 &A, const Vec2 &B, const Vec2 &C);

};

