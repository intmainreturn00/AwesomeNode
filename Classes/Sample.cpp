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

    auto screen = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 center(screen.width / 2, screen.height / 2);
    Vec2 quarter(screen.width / 4, screen.height / 4);
    Vec2 sixth(screen.width / 6, screen.height / 6);

    node = AwesomeNode::create();
    background = AwesomeNode::create();

    addChild(background);
    addChild(node);

    int n = 50;
    pts = PointArray::create(n);
    pts2 = PointArray::create(n);
    pts->retain();
    pts2->retain();
    auto defY = (int) (center.y + sixth.y);
    auto defY2 = (int) (center.y - sixth.y);
    auto dev = sixth.y;
    for (int i = 0; i < n; ++i) {
        pts->addControlPoint(Vec2(
                sixth.x + i * (screen.width - 2 * sixth.x) / n,
                RandomHelper::random_real<float>(defY - dev, defY + dev)));
        pts2->addControlPoint(Vec2(
                sixth.x + i * (screen.width - 2 * sixth.x) / n,
                RandomHelper::random_real<float>(defY2 - dev, defY2 + dev)));
    }

    background->drawSolidRect(origin, Vec2(screen.width, screen.height), Color4F::WHITE);
    drawGrid(Vec2(sixth.x, screen.height - sixth.y), Vec2(screen.width - sixth.x, sixth.y));

    node->drawTriangle(
            Vec2(origin.x, screen.height - quarter.y),
            Vec2(quarter.x / 2, screen.height),
            Vec2(quarter.x, screen.height - quarter.y),
            Color4F::GREEN, Color4F::RED, Color4F::BLUE);

    node->drawALine(Vec2(sixth.x, defY), Vec2(screen.width - sixth.x, defY), 1, Color4F::BLUE);
    node->drawACardinalSpline(pts, 0.5, 360, 4, Color4F::BLACK);

    node->drawALine(Vec2(sixth.x, defY2), Vec2(screen.width - sixth.x, defY2), 1, Color4F::BLUE);
    node->drawACardinalSpline(pts2, 0.5, 360, 2, Color4F::BLACK);

    node->drawDashDottedLine(
            Vec2(sixth.x - 20, center.y + sixth.y),
            Vec2(sixth.x - 20, center.y - sixth.y),
            2, 10, Color4F::ORANGE);

    node->drawDashDottedLine(
            Vec2(sixth.x - 40, center.y + sixth.y),
            Vec2(sixth.x - 40, center.y - sixth.y),
            4, 15, Color4F::ORANGE);

    node->drawDashedLine(
            Vec2(sixth.x - 60, center.y + sixth.y),
            Vec2(sixth.x - 60, center.y - sixth.y),
            7, 20, Color4F::ORANGE);



    //scheduleUpdate();
    return true;
}

void Sample::update(float dt) {
    //Node::update(dt);

}

void Sample::drawGrid(Vec2 A, Vec2 B) {
    node->drawRect(A, B, Color4F::GRAY);
    int n = 5;
    float stepX = (B.x - A.x) / n;
    float stepY = (A.y - B.y) / n;
    for (int i = 1; i < n; ++i) {
        node->drawDashedLine(Vec2(A.x + stepX * i, A.y), Vec2(A.x + stepX * i, B.y), 0.5, 10,
                             Color4F::GRAY);
    }

    for (int i = 1; i < n; ++i) {
        node->drawDashedLine(Vec2(A.x, A.y - stepY * i), Vec2(B.x, A.y - stepY * i), 0.5, 10,
                             Color4F::GRAY);
    }
}


