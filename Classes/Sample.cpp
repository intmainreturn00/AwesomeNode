#include "Sample.h"
#include "AwesomeNode.h"

USING_NS_CC;

Scene *Sample::createScene() {
	return Sample::create();
}

bool Sample::init() {

	if (!Scene::init()) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vec2 center(visibleSize.width / 2, visibleSize.height / 2);

	node = AwesomeNode::create(10, 20);

	addChild(node);

	/*node->drawTriangle(Vec2(200, 100), Vec2(400, 100), Vec2(200, 300),
		Color4B::RED, Color4B::GREEN, Color4B::BLUE);*/

	node->drawLineSegment(Vec2(200, 300), Vec2(200, 200), Vec2(200, 100), 
		Vec2(400, 100), Vec2(400, 200), Vec2(350, 300), Color4B::GREEN);

	return true;
}