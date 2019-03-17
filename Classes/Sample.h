#pragma once

#include "cocos2d.h"
#include "AwesomeNode.h"

#include "ui/CocosGUI.h"
using namespace ui;

class Sample : public cocos2d::Scene {
public:
    static cocos2d::Scene *createScene();

    bool init() override;

    CREATE_FUNC(Sample);

    void click();

    PointArray *pts = nullptr;
    AwesomeNode *awesomeNode = nullptr;
    DrawNode *justNode = nullptr;
};

