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

    CREATE_FUNC(AwesomeNode);

    bool init() override;

    /**draw triangle when each vertex can has its own color and opacity.*/
    void drawTriangle(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3,
                      const Color4F &color1, const Color4F &color2, const Color4F &color3);

    /** draw anti-aliased line */
    void drawALine(const Vec2 &A, const Vec2 &B, float w, const Color4F &color);

    /** draw anti-aliased cardinal spline with fixed math
     * (see https://discuss.cocos2d-x.org/t/catmull-rom-cardinal-spline-interpolation-problem/4586) */
    void drawACardinalSpline(PointArray *config, float tension, unsigned int segments,
                             float w, const Color4F &color);

    /** draw anti-aliased cardinal spline with feelings to bottom line*/
    void drawAFilledCardinalSpline(PointArray *config, float tension, unsigned int segments,
                             float w, const Color4F &color, float bottomY, const Color4F &fill);


    void drawADashDottedLine(const Vec2 &from, const Vec2 &to, float w, float dashSize,
                             const Color4F &color);

    void drawADashedLine(const Vec2 &from, const Vec2 &to, float w, float dashSize,
                         const Color4F &color);

    void drawSolidRect(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3, const Vec2& p4, const Color4F &color);

    void drawSolidRect(const Vec2 &origin, const Vec2 &destination, const Color4F &color);

private:

    static const int LINE_SIZE_THRESHOLD;

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

    void drawLineSegment(const lineSegment &segment, const Color4F &color);

    lineJoint calculateLineJoint(const Vec2 &A, const Vec2 &B, const Vec2 &C, float w);

    lineSegment calculateLineSegment(const Vec2 &A, const Vec2 &B, float w);

    Vec2 *calculateVertices(PointArray *config, float tension, unsigned int segments);

    void tessellation(Vec2 *vertices, unsigned int segments, float w, const Color4F &color);

    void highlightTriangle(const Vec2 &A, const Vec2 &B, const Vec2 &C);

};

