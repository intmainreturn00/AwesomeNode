#pragma once

#include "cocos2d.h"
#include "AwesomeNode.h"

class Sample : public cocos2d::Scene {
public:
    static cocos2d::Scene *createScene();

    virtual bool init();

    CREATE_FUNC(Sample);

    AwesomeNode *node = nullptr;
	PointArray *pts = nullptr;
};

