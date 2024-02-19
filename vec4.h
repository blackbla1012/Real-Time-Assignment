#pragma once

#include "vec3.h"

struct Vec4 {
	float x;
	float y;
	float z;
	float w;

	// Constructors
	Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	Vec4(float x) : x(x), y(x), z(x), w(x) {}
	Vec4(Vec3 xyz, float _w) {
		x = xyz.x;
		y = xyz.y;
		z = xyz.z;
		w = _w;
	}

	// Assignment operator
	Vec4& operator=(const Vec4& other) {
		if (this != &other) {
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}
		return *this;
	}

	Vec3 project() const {
		return Vec3(x / w, y / w, z / w);
	}
};
