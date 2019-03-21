#pragma once

#include "cocos2d.h"
#include "AwesomeNode.h"

USING_NS_CC;

class Sample : public cocos2d::Scene {
public:
    static cocos2d::Scene *createScene();

    bool init() override;

    CREATE_FUNC(Sample);

private:

    /** draw a grid from upper left (A) to down right (B) corners */
    void drawGrid(Vec2 A, Vec2 B);

    void generateDataPoints();

    void drawEverything();

    PointArray *pts = nullptr;
    PointArray *pts2 = nullptr;
    AwesomeNode *node = nullptr;

    Size screen;
    Vec2 origin, center, sixth;
    float defY, defY2, dev;
    const int n = 50;
    const int grid = 10;
    const int margin = 20;

    const Color4F BLACK = Color4F(Color4B(21, 23, 33, 255));
    const Color4F GREY = Color4F(Color4B(34, 38, 50, 255));
    const Color4F GREEN = Color4F(Color4B(64, 209, 163, 255));
    const Color4F GREEN_SH = Color4F(Color4B(27, 67, 63, 255));
    const Color4F RED = Color4F(Color4B(214, 41, 84, 255));
    const Color4F RED_SH = Color4F(Color4B(68, 27, 45, 255));

};

