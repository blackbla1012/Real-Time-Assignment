#pragma once

#include "vec3.h"
#include <cmath>

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

	float length() const {
		return std::sqrt(x * x + y * y + z * z + w * w);
	}

	Vec4 normalize() const {
		float l = length();
		if (l == 0) return Vec4(0.0f, 0.0f, 0.0f, 0.0f);

		return Vec4(x / l, y / l, z / l, w / l);
	}

	// 计算叉积
	Vec4 cross(const Vec4& other) const {
		return Vec4(
			y * other.z - z * other.y,  // X 分量
			z * other.x - x * other.z,  // Y 分量
			x * other.y - y * other.x,  // Z 分量
			1.0f                        // W 分量，通常设为 1.0
		);
	}

	//计算点积
	float dot(const Vec4& other) const {
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}

	Vec4 operator+(const Vec4& other) const {
		return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	Vec4 operator-(const Vec4& other) const {
		return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
	}

	Vec4 operator*(const float& n) const {
		return Vec4(x * n, y * n, z * n, w * n);
	}

	
};
