#include "AwesomeNode.h"


bool AwesomeNode::init() {
    return DrawNode::init();
}


void AwesomeNode::setOpacity(GLubyte opacity) {
    this->opacity = opacity;
    _displayedOpacity = _realOpacity = opacity;
    if (_bufferCount) {
        for (int i = 0; i < _bufferCount; i++) {
            _buffer[i].colors.a = opacity;
        }
    }

    if (_bufferCountGLPoint) {
        for (int i = 0; i < _bufferCountGLPoint; i++) {
            _bufferGLPoint[i].colors.a = opacity;
        }
    }

    if (_bufferCountGLLine) {
        for (int i = 0; i < _bufferCountGLLine; i++) {
            _bufferGLLine[i].colors.a = opacity;
        }
        _dirtyGLLine = true;
    }
    _dirty = true;
}


void AwesomeNode::drawTriangle(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3,
                               const Color4B &color1, const Color4B &color2,
                               const Color4B &color3) {

    unsigned int vertex_count = 3;
    ensureCapacity(vertex_count);

    V2F_C4B_T2F a = {Vec2(p1.x, p1.y), color1, Tex2F(0.0, 0.0)};
    V2F_C4B_T2F b = {Vec2(p2.x, p2.y), color2, Tex2F(0.0, 0.0)};
    V2F_C4B_T2F c = {Vec2(p3.x, p3.y), color3, Tex2F(0.0, 0.0)};

    V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *) (_buffer + _bufferCount);
    V2F_C4B_T2F_Triangle triangle = {a, b, c};
    triangles[0] = triangle;

    _bufferCount += vertex_count;
    _dirty = true;
}