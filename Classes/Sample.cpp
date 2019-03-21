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
    sixth = Vec2(screen.width / 6, screen.height / 6);

    defY = (int) (center.y + sixth.y);
    defY2 = (int) (center.y - sixth.y);
    dev = sixth.y;

    node = AwesomeNode::create();
    addChild(node);

    pts = PointArray::create(n);
    pts2 = PointArray::create(n);
    pts->retain();
    pts2->retain();
    for (int i = 0; i < n; ++ i) {
        pts->insertControlPoint(Vec2(0, 0), i);
        pts2->insertControlPoint(Vec2(0, 0), i);
    }

    runAction(RepeatForever::create(
            Sequence::create(
                    CallFunc::create([&]() {
                        generateDataPoints();
                        drawEverything();
                    }),
                    DelayTime::create(4),
                    nullptr))
    );


    return true;
}

void Sample::drawGrid(Vec2 A, Vec2 B) {
    node->drawSolidRect(A, B, BLACK);
    float stepX = (B.x - A.x) / grid;
    float stepY = (A.y - B.y) / grid;
    for (int i = 1; i < grid; ++i) {
        node->drawALine(Vec2(A.x + stepX * i, A.y), Vec2(A.x + stepX * i, B.y), 1, GREY);
    }
    for (int i = 1; i < grid; ++i) {
        node->drawALine(Vec2(A.x, A.y - stepY * i), Vec2(B.x, A.y - stepY * i), 1, GREY);
    }
}

void Sample::generateDataPoints() {
    for (int i = 0; i < n; ++i) {
        pts->replaceControlPoint(Vec2(
                margin + i * (screen.width - 3 * margin) / n,
                RandomHelper::random_real<float>(defY - dev, defY + dev)), i);
        pts2->replaceControlPoint(Vec2(
                margin + i * (screen.width - 3 * margin) / n,
                RandomHelper::random_real<float>(defY2 - dev, defY2 + dev)), i);
    }
}

void Sample::drawEverything() {
    node->clear();

    node->drawSolidRect(origin, Vec2(screen.width, screen.height), GREY);
    drawGrid(Vec2(margin, screen.height - margin), Vec2(screen.width - margin, margin));

    auto level1 = (int) defY + RandomHelper::random_int(2, 12);
    node->drawDashedLine(Vec2(margin, level1),
                         Vec2(screen.width - margin, level1), 1, 5, GREEN);
    auto level2 = (int) defY2 + RandomHelper::random_int(2, 12);
    node->drawDashedLine(Vec2(margin, level2),
                         Vec2(screen.width - margin, level2), 1, 5, RED);

    node->drawAFilledCardinalSpline(pts, 0.5, 360, 5, RED, center.y, RED_SH);
    node->drawAFilledCardinalSpline(pts2, 0.5, 360, 2, GREEN, sixth.y, GREEN_SH);

    int i1 = RandomHelper::random_int(0, n - 1);
    int i2 = RandomHelper::random_int(0, n - 1);
    node->drawDot(pts->getControlPointAtIndex(i1), 7, Color4F(0, 1, 0, 0.3));
    node->drawDot(pts->getControlPointAtIndex(i1), 4, Color4F::GREEN);

    node->drawDot(pts2->getControlPointAtIndex(i2), 7, Color4F(0, 1, 0, 0.3));
    node->drawDot(pts2->getControlPointAtIndex(i2), 4, Color4F::GREEN);

    node->drawDashDottedLine(Vec2(screen.width - 2 * margin, margin),
                             Vec2(screen.width - 2 * margin, screen.height - margin), 2, 7, RED_SH);
}


