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
	const Color4B &color1, const Color4B &color2, const Color4B &color3) {

	unsigned int vertex_count = 3;
	ensureCapacity(vertex_count);

	V2F_C4B_T2F a = { Vec2(p1.x, p1.y), color1, Tex2F(0.0, 0.0) };
	V2F_C4B_T2F b = { Vec2(p2.x, p2.y), color2, Tex2F(0.0, 0.0) };
	V2F_C4B_T2F c = { Vec2(p3.x, p3.y), color3, Tex2F(0.0, 0.0) };

	V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *)(_buffer + _bufferCount);
	V2F_C4B_T2F_Triangle triangle = { a, b, c };
	triangles[0] = triangle;

	_bufferCount += vertex_count;
	_dirty = true;

#ifdef AWESOMEDEBUG
	drawLine(p1, p2, Color4F::MAGENTA);
	drawLine(p2, p3, Color4F::MAGENTA);
	drawLine(p3, p1, Color4F::MAGENTA);
#endif // AWESOMEDEBUG

}

void AwesomeNode::drawALine(const Vec2 & A, const Vec2 & B, const Color4B & color, int w)
{
	Vec2 p(B.x - A.x, B.y - A.y);
	p.normalize();
	Vec2 p1(-p.y, p.x), p2(p.y, -p.x);
	Vec2 A1(A + p1 * w / 2), A2(A + p2 * w / 2);
	Vec2 B1(B + p1 * w / 2), B2(B + p2 * w / 2);

	drawLineSegment(A1, A, A2, B2, B, B1, color);
}

void AwesomeNode::drawLineSegment(const Vec2 & p1, const Vec2 & p2, const Vec2 &p3,
	const Vec2 & p4, const Vec2 & p5, const Vec2 & p6, const Color4B &color)
{
	Color4B border(color);
	border.a = 0;

	drawTriangle(p1, p6, p2, border, border, color);
	drawTriangle(p2, p6, p5, color, border, color);
	drawTriangle(p2, p5, p3, color, color, border);
	drawTriangle(p3, p5, p4, border, color, border);
}

