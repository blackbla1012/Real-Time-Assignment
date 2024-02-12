#ifndef VEC4_H
#define VEC4_H

struct Vec4 {
	float x;
	float y;
	float z;
	float w;

	// Constructors
	Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	Vec4(float x) : x(x), y(x), z(x), w(x) {}
};

#endif // !VEC4_H