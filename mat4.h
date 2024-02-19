#pragma once
#include <iostream>
#include <cmath>

#include "vec3.h"
#include "vec4.h"

class Mat4 {
public:
	float data[4][4];

	//construction
	Mat4() {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				data[i][j] = (i == j) ? 1.0f : 0.0f;//uniform matrix
			}
		}
	}

	static Mat4 translate(Vec3 t) {
		Mat4 r;
		r.data[3][0] = t.x;
		r.data[3][1] = t.y;
		r.data[3][2] = t.z;
		return r;
	}

	static Mat4 rotate(Vec4 rotation) {
		Mat4 r;
		float xx = rotation.x * rotation.x;
		float yy = rotation.y * rotation.y;
		float zz = rotation.z * rotation.z;
		float xy = rotation.x * rotation.y;
		float xz = rotation.x * rotation.z;
		float yz = rotation.y * rotation.z;
		float wx = rotation.w * rotation.x;
		float wy = rotation.w * rotation.y;
		float wz = rotation.w * rotation.z;

		r.data[0][0] = 1.0f - 2.0f * (yy + zz);
		r.data[0][1] = 2.0f * (xy - wz);
		r.data[0][2] = 2.0f * (xz + wy);

		r.data[1][0] = 2.0f * (xy + wz);
		r.data[1][1] = 1.0f - 2.0f * (xx + zz);
		r.data[1][2] = 2.0f * (yz - wx);

		r.data[2][0] = 2.0f * (xz - wy);
		r.data[2][1] = 2.0f * (yz + wx);
		r.data[2][2] = 1.0f - 2.0f * (xx + yy);
		return r;
	}

	static Mat4 scale(Vec3 scale) {
		Mat4 r;
		r.data[0][0] *= scale.x;
		r.data[1][1] *= scale.y;
		r.data[2][2] *= scale.z;
		return r;
	}

	Mat4 operator*(const Mat4& other) const {
		Mat4 r;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				r.data[i][j] = 0.0f;
				for (int k = 0; k < 4; ++k) {
					r.data[i][j] += data[i][k] * other.data[k][j];
				}
			}
		}
		return r;
	}

	Vec4 operator*(const Vec4& vec) const {
		Vec4 r;
		r.x = data[0][0] * vec.x + data[0][1] * vec.y + data[0][2] * vec.z + data[0][3] * vec.w;
		r.y = data[1][0] * vec.x + data[1][1] * vec.y + data[1][2] * vec.z + data[1][3] * vec.w;
		r.z = data[2][0] * vec.x + data[2][1] * vec.y + data[2][2] * vec.z + data[2][3] * vec.w;
		r.w = data[3][0] * vec.x + data[3][1] * vec.y + data[3][2] * vec.z + data[3][3] * vec.w;
		return r;
	}

	Vec3 operator*(const Vec3& vec) const {
		return operator*(Vec4(vec, 1.0f)).project();
	}

	static Mat4 transpose(const Mat4& m) {
		Mat4 r;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				r.data[i][j] = m.data[j][i];
			}
		}
		return r;
	}

	static Mat4 inverse(const Mat4& m) {
		Mat4 inv;

		float det = m.data[0][0] * (m.data[1][1] * m.data[2][2] - m.data[1][2] * m.data[2][1])
			- m.data[0][1] * (m.data[1][0] * m.data[2][2] - m.data[1][2] * m.data[2][0])
			+ m.data[0][2] * (m.data[1][0] * m.data[2][1] - m.data[1][1] * m.data[2][0]);

		if (det == 0) {
			std::cerr << "Cannot invert matrix. Determinant is zero." << std::endl;
			return inv;
		}

		float invDet = 1.0f / det;

		inv.data[0][0] = (m.data[1][1] * m.data[2][2] - m.data[1][2] * m.data[2][1]) * invDet;
		inv.data[0][1] = -(m.data[0][1] * m.data[2][2] - m.data[0][2] * m.data[2][1]) * invDet;
		inv.data[0][2] = (m.data[0][1] * m.data[1][2] - m.data[0][2] * m.data[1][1]) * invDet;
		inv.data[0][3] = 0.0f;

		inv.data[1][0] = -(m.data[1][0] * m.data[2][2] - m.data[1][2] * m.data[2][0]) * invDet;
		inv.data[1][1] = (m.data[0][0] * m.data[2][2] - m.data[0][2] * m.data[2][0]) * invDet;
		inv.data[1][2] = -(m.data[0][0] * m.data[1][2] - m.data[0][2] * m.data[1][0]) * invDet;
		inv.data[1][3] = 0.0f;

		inv.data[2][0] = (m.data[1][0] * m.data[2][1] - m.data[1][1] * m.data[2][0]) * invDet;
		inv.data[2][1] = -(m.data[0][0] * m.data[2][1] - m.data[0][1] * m.data[2][0]) * invDet;
		inv.data[2][2] = (m.data[0][0] * m.data[1][1] - m.data[0][1] * m.data[1][0]) * invDet;
		inv.data[2][3] = 0.0f;

		inv.data[3][0] = -(m.data[3][0] * inv.data[0][0] + m.data[3][1] * inv.data[1][0] + m.data[3][2] * inv.data[2][0]);
		inv.data[3][1] = -(m.data[3][0] * inv.data[0][1] + m.data[3][1] * inv.data[1][1] + m.data[3][2] * inv.data[2][1]);
		inv.data[3][2] = -(m.data[3][0] * inv.data[0][2] + m.data[3][1] * inv.data[1][2] + m.data[3][2] * inv.data[2][2]);
		inv.data[3][3] = 1.0f;

		return inv;
	}
};