/*
* AwesomeNode.h
*
*  Created on: October 1, 2018
*      Author: Andrey Suvorov
*/

#pragma once

#include "cocos2d.h"

USING_NS_CC;

#define AWESOMEDEBUG

class AwesomeNode : public DrawNode {

public:

	void setOpacity(GLubyte opacity);

	AwesomeNode(int width, int borderWidth) : W(width), w(borderWidth),
		DrawNode() { /*_blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;*/
	}

	int W, w;

	static AwesomeNode *create(int width = 5, int borderWidth = 2) {
		AwesomeNode *ret = new(std::nothrow) AwesomeNode(width, borderWidth);
		if (ret && ret->init()) {
			ret->autorelease();
			return ret;
		}
		else {
			delete ret;
			return nullptr;
		}
	}

	virtual bool init();

	/**draw triangle when each vertex can has its own color.*/
	void drawTriangle(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3,
		const Color4B &color1, const Color4B &color2, const Color4B &color3);

	/** draw tesselated (?) line segment */
	void drawLineSegment(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3,
		const Vec2 &p4, const Vec2 &p5, const Vec2 &p6, Color4B color);

private:
	GLubyte opacity;

	
	
};

