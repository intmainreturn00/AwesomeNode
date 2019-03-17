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

    awesomeNode = AwesomeNode::create();
    justNode = DrawNode::create();

    addChild(awesomeNode);
    addChild(justNode);

    auto button = Button::create("normal_image.png", "selected_image.png", "disabled_image.png");

    button->setTitleText("awesome Node");
    button->setTitleColor(Color3B::BLACK);
    button->setZoomScale(1.2);

    // set padding
    button->ignoreContentAdaptWithSize(false);
    button->setContentSize(Size(
            button->getTitleRenderer()->getContentSize().width + 15,
            button->getTitleRenderer()->getContentSize().height + 15)
    );

    button->setPosition(Vec2(visibleSize.width - 50, 50));

    // set margin
    LinearLayoutParameter* lp = LinearLayoutParameter::create();
    button->setLayoutParameter(lp);
    lp->setGravity(LinearLayoutParameter::LinearGravity::LEFT);
    lp->setMargin(Margin(20, 20, 0, 0));


    std::function<void()> fun = std::bind(&Sample::click, this);
    button->addTouchEventListener([fun](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            fun();
        }
    });

    addChild(button);


    pts = PointArray::create(6);
    pts->retain();
    auto defY = (int)center.y;
    pts->addControlPoint(Vec2(50, defY + 50));
    pts->addControlPoint(Vec2(100, defY - 50));
    pts->addControlPoint(Vec2(150, defY + 50));
    pts->addControlPoint(Vec2(200, defY - 50));
    pts->addControlPoint(Vec2(250, defY + 50));
    pts->addControlPoint(Vec2(350, defY + 50));

    justNode->drawCardinalSpline(pts, 0.5, 360, Color4F::GREEN);
    awesomeNode->drawACardinalSpline(pts, 0.5, 360, Color4B::GREEN, 2);

    justNode->drawLine(Vec2(50, 50), Vec2(100, 100), Color4F::GREEN);
    awesomeNode->drawALine(Vec2(50, 50), Vec2(100, 100), Color4B::GREEN, 2);

    justNode->drawTriangle(Vec2(200, 200), Vec2(250, 250), Vec2(300, 200), Color4F::GREEN);
    awesomeNode->drawTriangle(Vec2(200, 200), Vec2(250, 250), Vec2(300, 200),
            Color4B::GREEN, Color4B::RED, Color4B(0xff, 0, 0, 70));

    awesomeNode->setVisible(false);


    return true;
}

void Sample::click() {
    CCLOG("click");
    awesomeNode->setVisible(!awesomeNode->isVisible());
    justNode->setVisible(!justNode->isVisible());

    awesomeNode->clear();
    justNode->clear();

    justNode->drawCardinalSpline(pts, 0.5, 360, Color4F::GREEN);
    awesomeNode->drawACardinalSpline(pts, 0.5, 360, Color4B::GREEN, 2);

    justNode->drawLine(Vec2(50, 50), Vec2(100, 100), Color4F::GREEN);
    awesomeNode->drawALine(Vec2(50, 50), Vec2(100, 100), Color4B::GREEN, 4);

    justNode->drawTriangle(Vec2(200, 200), Vec2(250, 250), Vec2(300, 200), Color4F::GREEN);
    awesomeNode->drawTriangle(Vec2(200, 200), Vec2(250, 250), Vec2(300, 200),
                              Color4B::GREEN, Color4B::RED, Color4B(0xff, 0, 0, 70));
}

