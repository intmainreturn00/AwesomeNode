/*
* AwesomeNode.h
*
*  Created on: October 1, 2018
*      Author: Andrey Suvorov
*/

#pragma once

#include "cocos2d.h"

//#define AWESOMEDEBUG

class AwesomeNode : public cocos2d::DrawNode {

public:

    CREATE_FUNC(AwesomeNode);

    bool init() override;

    /**draw triangle when each vertex can has its own color and opacity.*/
    void drawTriangle(const cocos2d::Vec2 &p1,
                      const cocos2d::Vec2 &p2,
                      const cocos2d::Vec2 &p3,
                      const cocos2d::Color4F &color1,
                      const cocos2d::Color4F &color2,
                      const cocos2d::Color4F &color3);

    /** draw anti-aliased line */
    void drawALine(const cocos2d::Vec2 &A,
                   const cocos2d::Vec2 &B,
                   float w,
                   const cocos2d::Color4F &color);

    /** draw anti-aliased cardinal spline with fixed math
     * (see https://discuss.cocos2d-x.org/t/catmull-rom-cardinal-spline-interpolation-problem/4586) */
    void drawACardinalSpline(cocos2d::PointArray *config,
                             float tension,
                             unsigned int segments,
                             float w,
                             const cocos2d::Color4F &color);

    /** draw anti-aliased cardinal spline with feelings to bottom line*/
    void drawAFilledCardinalSpline(cocos2d::PointArray *config,
                                   float tension,
                                   unsigned int segments,
                                   float w,
                                   const cocos2d::Color4F &color,
                                   float bottomY,
                                   const cocos2d::Color4F &fill);


    void drawADashDottedLine(const cocos2d::Vec2 &from,
                             const cocos2d::Vec2 &to,
                             float w,
                             float dashSize,
                             const cocos2d::Color4F &color);

    void drawADashedLine(const cocos2d::Vec2 &from,
                         const cocos2d::Vec2 &to,
                         float w,
                         float dashSize,
                         const cocos2d::Color4F &color);

    void drawSolidRect(const cocos2d::Vec2 &p1,
                       const cocos2d::Vec2 &p2,
                       const cocos2d::Vec2 &p3,
                       const cocos2d::Vec2 &p4,
                       const cocos2d::Color4F &color);

    void drawSolidRect(const cocos2d::Vec2 &origin,
                       const cocos2d::Vec2 &destination,
                       const cocos2d::Color4F &color);

private:

    static const int LINE_SIZE_THRESHOLD;

    struct lineSegment {
        cocos2d::Vec2 A, A1, A2;
        cocos2d::Vec2 B, B1, B2;
    };

    struct lineJoint {
        lineSegment segment;
        cocos2d::Vec2 B3, B4;
        cocos2d::Vec2 K, K3;
        bool up;
    };

    void drawLineSegment(const lineSegment &segment,
                         const cocos2d::Color4F &color);

    lineJoint calculateLineJoint(const cocos2d::Vec2 &A,
                                 const cocos2d::Vec2 &B,
                                 const cocos2d::Vec2 &C,
                                 float w);

    lineSegment calculateLineSegment(const cocos2d::Vec2 &A,
                                     const cocos2d::Vec2 &B,
                                     float w);

    cocos2d::Vec2 *calculateVertices(cocos2d::PointArray *config,
                                     float tension,
                                     unsigned int segments);

    void tessellation(cocos2d::Vec2 *vertices,
                      unsigned int segments,
                      float w,
                      const cocos2d::Color4F &color);

    void highlightTriangle(const cocos2d::Vec2 &A,
                           const cocos2d::Vec2 &B,
                           const cocos2d::Vec2 &C);

};

