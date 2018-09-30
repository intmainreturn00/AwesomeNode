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

    AwesomeNode(int width) : DrawNode() { /*_blendFunc = BlendFunc::DISABLE;*/ }

    static AwesomeNode *create(int width = 2) {
        AwesomeNode *pRet = new(std::nothrow) AwesomeNode(width);
        if (pRet && pRet->init()) {
            pRet->autorelease();
            return pRet;
        } else {
            delete pRet;
            return nullptr;
        }
    }

    virtual bool init();

    void drawTriangle(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3,
                      const Color4B &color1, const Color4B &color2, const Color4B &color3);


private:
    GLubyte opacity;
};

