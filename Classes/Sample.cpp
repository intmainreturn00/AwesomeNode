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

    auto label = Label::createWithTTF("Awesome Node Usage Example", "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(origin.x + visibleSize.width / 2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    this->addChild(label, 1);

    // triangle with vertices with different colors
    AwesomeNode *drawer = AwesomeNode::create();
    drawer->drawTriangle(Vec2(center.x, center.y),
                         Vec2(center.x + 100, center.y + 100),
                         Vec2(center.x + 200, center.y),
                         Color4B::BLUE, Color4B(0, 0, 255, 0), Color4B::BLUE);
    this->addChild(drawer);

    // transparent triangle
    AwesomeNode *drawer2 = AwesomeNode::create();
    drawer2->setOpacity(255 / 4);
    drawer2->drawTriangle(Vec2(center.x - 100, center.y),
                          Vec2(center.x, center.y + 100),
                          Vec2(center.x + 100, center.y),
                          Color4B::RED, Color4B(255, 0, 0, 0), Color4B::RED);
    this->addChild(drawer2);


    node = AwesomeNode::create(20, 10);
    pts = PointArray::create(6);
    pts->retain();
    int defY = center.y;
    pts->addControlPoint(Vec2(0, defY + 50));
    pts->addControlPoint(Vec2(100, defY - 50));
    pts->addControlPoint(Vec2(200, defY + 50));
    pts->addControlPoint(Vec2(300, defY - 50));
    pts->addControlPoint(Vec2(400, defY + 50));
    pts->addControlPoint(Vec2(500, defY - 50));


    drawPts();

    EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [&](Touch *touch, Event *event) {
        return true;
    };

    listener->onTouchMoved = [&](Touch *touch, Event *event) {
        CCLOG("%f %f", touch->getLocation().x, touch->getLocation().y);
        Vec2 local = convertToNodeSpace(touch->getLocation());
        CCLOG("%f %f", local.x, local.y);

        CCLOG("pts count = %d", pts->count());

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



        //return true;
    };

    listener->onTouchEnded = [&](Touch *touch, Event *event) {
        //return true;
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


//    runAction(RepeatForever::create(
//            Sequence::create(
//                    CallFunc::create([&]() {
//                        // update vertices..
//                        for (int i = 0; i < pts->count(); i += 2) {
//                            Vec2 p = pts->getControlPointAtIndex(i);
//                            p.y += step;
//                            pts->replaceControlPoint(p, i);
//                        }
//                        for (int i = 1; i < pts->count(); i += 2) {
//                            Vec2 p = pts->getControlPointAtIndex(i);
//                            p.y -= step;
//                            pts->replaceControlPoint(p, i);
//                        }
//                        counter++;
//                        if (counter > 100) {
//                            step = -step;
//                            counter = 0;
//                        }
//                        drawPts();
//
//                    }),
//                    DelayTime::create(0.05),
//                    nullptr))
//    );

    this->addChild(node);


    return true;
}

void Sample::drawPts() {
    node->clear();
    auto color = Color4F::RED;
    //color.a = 150;
    node->drawCardinalSpline(pts, tension, segments, color);
    /*for (int i = 0; i < pts->count(); ++i) {
        node->drawPoint(pts->getControlPointAtIndex(i), 10, Color4F::BLUE);
    }*/
}
