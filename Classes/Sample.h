#pragma once

#include "cocos2d.h"
#include "AwesomeNode.h"

class Sample : public cocos2d::Scene {
public:
    static cocos2d::Scene *createScene();

    virtual bool init();

    CREATE_FUNC(Sample);

    void drawPts();

    AwesomeNode *node = nullptr;
    PointArray *pts = nullptr;
    int step = -1;
    int counter = 0;
    float tension = 0;
    int segments = 120;

};

