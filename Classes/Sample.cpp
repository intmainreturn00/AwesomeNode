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

    node = AwesomeNode::create();

    addChild(node);

    /*node->drawTriangle(Vec2(200, 100), Vec2(400, 100), Vec2(200, 300),
        Color4B::RED, Color4B::GREEN, Color4B::BLUE);*/

    /*node->drawLineSegment(Vec2(200, 300), Vec2(200, 200), Vec2(200, 100),
        Vec2(400, 100), Vec2(400, 200), Vec2(350, 300), Color4B::GREEN);*/


    //node->drawALine(Vec2(100, 200), Vec2(400, 200), Color4B::GREEN, 50);

//	pts = PointArray::create(5);
//	pts->retain();
//	pts->addControlPoint(Vec2(50, 50));
//	pts->addControlPoint(Vec2(150, 50));
//	pts->addControlPoint(Vec2(250, 150));
//	pts->addControlPoint(Vec2(350, 150));
//	pts->addControlPoint(Vec2(400, 200));
//
//	node->drawACardinalSpline(pts, 0.5, 30, Color4B::GREEN, 50);
//
//	node->drawALine(Vec2(50, 200), Vec2(350, 200), Color4B::GREEN, 20);

    pts = PointArray::create(6);
    pts->retain();
    int defY = center.y;
    pts->addControlPoint(Vec2(50, defY + 50));
    pts->addControlPoint(Vec2(100, defY - 50));
    pts->addControlPoint(Vec2(150, defY + 50));
    pts->addControlPoint(Vec2(200, defY - 50));
    pts->addControlPoint(Vec2(250, defY + 50));
    pts->addControlPoint(Vec2(350, defY + 50));


    drawPts();

    EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [&](Touch *touch, Event *event) {
        return true;
    };

    listener->onTouchMoved = [&](Touch *touch, Event *event) {
        Vec2 local = convertToNodeSpace(touch->getLocation());

        // find closest to click point
        int closestIndx = -1;
        float closestDist = 100500;
        for (int i = 0; i < pts->count(); ++i) {
            float curDist = local.distance(pts->getControlPointAtIndex(i));
            if (curDist < closestDist) {
                closestIndx = i;
                closestDist = curDist;
            }
        }

        assert(closestIndx >= 0);
        assert(closestIndx < pts->count());

        // move it to the click location
        pts->replaceControlPoint(local, closestIndx);

        // redraw
        drawPts();
    };

    listener->onTouchEnded = [&](Touch *touch, Event *event) {
        //return true;
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


    return true;
}

void Sample::drawPts() {
    node->clear();
    node->drawACardinalSpline(pts, 0.5, 30, Color4B::RED, 30);
}
