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

    screen = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    center = Vec2(screen.width / 2, screen.height / 2);
    quarter = Vec2(screen.width / 4, screen.height / 4);
    sixth = Vec2(screen.width / 6, screen.height / 6);

    node = AwesomeNode::create();
    background = AwesomeNode::create();

    addChild(background);
    addChild(node);

    runAction(RepeatForever::create(
            Sequence::create(
                    CallFunc::create([&]() {
                        generateDataPoints();
                        drawEverything();
                    }),
                    DelayTime::create(4),
                    nullptr))
    );


    //scheduleUpdate();
    return true;
}

void Sample::drawGrid(Vec2 A, Vec2 B) {
    node->drawSolidRect(A, B, BLACK);
    int n = 10;
    float stepX = (B.x - A.x) / n;
    float stepY = (A.y - B.y) / n;
    for (int i = 1; i < n; ++i) {
        node->drawALine(Vec2(A.x + stepX * i, A.y), Vec2(A.x + stepX * i, B.y), 1, GREY);
    }
    for (int i = 1; i < n; ++i) {
        node->drawALine(Vec2(A.x, A.y - stepY * i), Vec2(B.x, A.y - stepY * i), 1, GREY);
    }
}

void Sample::generateDataPoints() {
    n = 50;
    pts = PointArray::create(n);
    pts2 = PointArray::create(n);
    pts->retain();
    pts2->retain();
    defY = (int) (center.y + sixth.y);
    defY2 = (int) (center.y - sixth.y);
    dev = sixth.y;
    for (int i = 0; i < n; ++i) {
        pts->addControlPoint(Vec2(
                20 + i * (screen.width - 60) / n,
                RandomHelper::random_real<float>(defY - dev, defY + dev)));
        pts2->addControlPoint(Vec2(
                20 + i * (screen.width - 60) / n,
                RandomHelper::random_real<float>(defY2 - dev, defY2 + dev)));
    }
}

void Sample::drawEverything() {
    background->clear();
    node->clear();

    background->drawSolidRect(origin, Vec2(screen.width, screen.height), GREY);
    drawGrid(Vec2(20, screen.height - 20), Vec2(screen.width - 20, 20));

    auto level1 = (int) defY + RandomHelper::random_int(2, 12);
    node->drawDashedLine(Vec2(20, level1),
                         Vec2(screen.width - 20, level1), 1, 5, GREEN);
    auto level2 = (int) defY2 + RandomHelper::random_int(2, 12);
    node->drawDashedLine(Vec2(20, level2),
                         Vec2(screen.width - 20, level2), 1, 5, RED);

    node->drawAFilledCardinalSpline(pts, 0.5, 360, 5, RED, center.y, RED_SH);
    node->drawAFilledCardinalSpline(pts2, 0.5, 360, 2, GREEN, sixth.y, GREEN_SH);

    int i1 = RandomHelper::random_int(0, n - 1);
    int i2 = RandomHelper::random_int(0, n - 1);
    node->drawDot(pts->getControlPointAtIndex(i1), 7, Color4F(0, 1, 0, 0.3));
    node->drawDot(pts->getControlPointAtIndex(i1), 4, Color4F::GREEN);

    node->drawDot(pts2->getControlPointAtIndex(i2), 7, Color4F(0, 1, 0, 0.3));
    node->drawDot(pts2->getControlPointAtIndex(i2), 4, Color4F::GREEN);

    node->drawDashDottedLine(Vec2(screen.width - 40, 20),
                             Vec2(screen.width - 40, screen.height - 20), 2, 7, RED_SH);
}


