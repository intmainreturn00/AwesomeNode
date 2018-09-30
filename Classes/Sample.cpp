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

    AwesomeNode *drawer = AwesomeNode::create();
    drawer->drawTriangle(Vec2(center.x, center.y),
                         Vec2(center.x + 100, center.y + 100),
                         Vec2(center.x + 200, center.y),
                         Color4B::BLUE, Color4B(0, 0, 255, 0), Color4B::BLUE);
    this->addChild(drawer);

    AwesomeNode *drawer2 = AwesomeNode::create();
    drawer2->setOpacity(255 / 4);
    drawer2->drawTriangle(Vec2(center.x - 100, center.y),
                          Vec2(center.x, center.y + 100),
                          Vec2(center.x + 100, center.y),
                          Color4B::RED, Color4B(255, 0, 0, 0), Color4B::RED);
    this->addChild(drawer2);


    return true;
}
