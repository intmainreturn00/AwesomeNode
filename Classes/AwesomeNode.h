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

	//AwesomeNode() : W(width), w(borderWidth),
	//	DrawNode() { /*_blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;*/
	//}

	CREATE_FUNC(AwesomeNode);

	/*static AwesomeNode *create(int width = 5, int borderWidth = 2) {
		AwesomeNode *ret = new(std::nothrow) AwesomeNode(width, borderWidth);
		if (ret && ret->init()) {
			ret->autorelease();
			return ret;
		}
		else {
			delete ret;
			return nullptr;
		}
	}*/

	virtual bool init();

	/**draw triangle when each vertex can has its own color.*/
	void drawTriangle(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3,
		const Color4B &color1, const Color4B &color2, const Color4B &color3);

	/** draw anti-aliased line */
	void drawALine(const Vec2 &A, const Vec2 &B, const Color4B &color, int w);

private:

	GLubyte opacity;

	//int W, w;

	/** draw tesselated (?) line segment */
	void drawLineSegment(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3,
		const Vec2 &p4, const Vec2 &p5, const Vec2 &p6, const Color4B &color);

};

