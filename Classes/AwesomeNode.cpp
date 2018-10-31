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

void AwesomeNode::drawACardinalSpline(PointArray * config, float tension, unsigned int segments, 
	const Color4B & color, int w)
{
	Vec2 *vertices = new(std::nothrow) Vec2[segments + 1];
	if (!vertices)
		return;

	ssize_t p;
	float lt;
	float deltaT = 1.0f / config->count();

	for (unsigned int i = 0; i < segments + 1; i++) {

		float dt = (float)i / segments;

		// border
		if (dt == 1) {
			p = config->count() - 1;
			lt = 1;
		}
		else {
			p = dt / deltaT;
			lt = (dt - deltaT * (float)p) / deltaT;
		}

		// Interpolate
		Vec2 pp0 = config->getControlPointAtIndex(p - 1);
		Vec2 pp1 = config->getControlPointAtIndex(p + 0);
		Vec2 pp2 = config->getControlPointAtIndex(p + 1);
		Vec2 pp3 = config->getControlPointAtIndex(p + 2);

		Vec2 newPos = ccCardinalSplineAt(pp0, pp1, pp2, pp3, tension, lt);
		vertices[i].x = newPos.x;
		vertices[i].y = newPos.y;
	}

	for (int i = 0; i < segments; ++i) {
		//drawPoint(vertices[i], 5, Color4F::RED);
		CCLOG("%f %f", vertices[i].x, vertices[i].y);
	}
	w = w / 2;

	Vec2 G, N, U;
	if (segments > 0)
	{
		Vec2 A(vertices[0]), B(vertices[1]), C(vertices[2]);

		Vec2 n1(A.y - B.y, B.x - A.x), n2(B.y - A.y, A.x - B.x);

		n1.normalize();
		n2.normalize();

		Vec2 A1(A.x + n1.x * w, A.y + n1.y * w),
			A2(A.x + n2.x * w, A.y + n2.y * w);

		G = A1;
		N = A;
		U = A2;
	}

	for (int i = 2; i < segments - 1; ++i) {
		//drawPoint(vertices[i], 5, Color4F::RED);
		CCLOG("%f %f", vertices[i].x, vertices[i].y);
		Vec2 A(vertices[i - 2]), B(vertices[i - 1]), C(vertices[i]);
		
		Vec2 n1(A.y - B.y, B.x - A.x), n2(B.y - A.y, A.x - B.x);
		Vec2 n3(B.y - C.y, C.x - B.x), n4(C.y - B.y, B.x - C.x);

		n1.normalize();
		n2.normalize();
		n3.normalize();
		n4.normalize();

		Vec2 A1(A.x + n1.x * w, A.y + n1.y * w),
			A2(A.x + n2.x * w, A.y + n2.y * w);
		Vec2 B1(B.x + n1.x * w, B.y + n1.y * w),
			B2(B.x + n2.x * w, B.y + n2.y * w),
			B3(B.x + n3.x * w, B.y + n3.y * w),
			B4(B.x + n4.x * w, B.y + n4.y * w);
		Vec2 C1(C.x + n3.x * w, C.y + n3.y * w),
			C2(C.x + n4.x * w, C.y + n4.y * w);

		Color4B border(color);
		border.a = 0;

		float angle1 = atan2(n1.x * n3.y - n1.y * n3.x, n1.x * n3.x + n1.y * n3.y);
		if (angle1 > 0) {
			// up case - inner. need to add join triangle to convex (down)
			//drawTriangle(B4, B2, B, Color4B::BLUE, Color4B::BLUE, Color4B::BLUE);
			//            drawTriangle(B4, B2, B, main, main, main);
			CCLOG("up");
			Vec2 b13((n1.x + n3.x), (n1.y + n3.y));
			b13.normalize();
			Vec2 K(B.x + b13.x * (w), B.y + b13.y * (w));
			//drawPoint(K, 5, Color4F::ORANGE);
			drawTriangle(B, B2, B4, color, border, border);

			// G N U B2 B K
			drawLineSegment(G, N, U, B2, B, K, color);
			G = K;
			N = B;
			U = B4;
		}
		else {
			// down case - inner. need to add join triangle to convex (up)
			//drawTriangle(B1, B3, B, Color4B::BLUE, Color4B::BLUE, Color4B::BLUE);
			//            drawTriangle(B1, B3, B, main, main, main);
			CCLOG("down");
			Vec2 b24((n2.x + n4.x), (n2.y + n4.y));
			b24.normalize();
			Vec2 K3(B.x + b24.x * (w), B.y + b24.y * (w));
			//drawPoint(K3, 5, Color4F::ORANGE);
			drawTriangle(B, B1, B3, color, border, border);

			// G N U K3 B B1
			drawLineSegment(G, N, U, K3, B, B1, color);
			G = B3;
			N = B;
			U = K3;
		}
		
		//drawALine(A, B, color, 30);
	}

	{
		Vec2 A(vertices[segments - 2]), B(vertices[segments-1]);

		Vec2 n1(A.y - B.y, B.x - A.x), n2(B.y - A.y, A.x - B.x);

		n1.normalize();
		n2.normalize();

		Vec2 B1(B.x + n1.x * w, B.y + n1.y * w),
			B2(B.x + n2.x * w, B.y + n2.y * w);

		drawLineSegment(G, N, U, B2, B, B1, color);
	}
	

	//drawPoly(vertices, segments + 1, false, color);

	CC_SAFE_DELETE_ARRAY(vertices);
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

