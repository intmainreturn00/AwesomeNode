#pragma once

#include "cocos2d.h"
#include "AwesomeNode.h"

#include "ui/CocosGUI.h"
using namespace ui;

class Sample : public cocos2d::Scene {
public:
    static cocos2d::Scene *createScene();

    bool init() override;
    void update(float dt) override;

    CREATE_FUNC(Sample);

private:

    /** draw a grid from upper left (A) to down right (B) corners */
    void drawGrid(Vec2 A, Vec2 B);

    PointArray *pts = nullptr;
    PointArray *pts2 = nullptr;
    AwesomeNode *node = nullptr;
    AwesomeNode *background = nullptr;
};

