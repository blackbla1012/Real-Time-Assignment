#pragma once

struct Vec3 {
	float x;
	float y;
	float z;

	//Constructors
	Vec3() : x(0.0f), y(0.0f), z(0.0f){}
	Vec3(float x, float y, float z) : x(x), y(y), z(z){}
	Vec3(float x) : x(x), y(x), z(x){}

    // Assignment operator
    Vec3& operator=(const Vec3& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
            z = other.z;
        }
        return *this;
    }

	float length() const {
		return std::sqrt(x * x + y * y + z * z);
	}

	Vec3 normalize() const {
		float l = length();
		if (l == 0) return Vec3(0.0f, 0.0f, 0.0f);

		return Vec3(x / l, y / l, z / l);
	}

    // 计算叉积
    Vec3 cross(const Vec3& other) const {
        return Vec3(
            y * other.z - z * other.y,  // X 分量
            z * other.x - x * other.z,  // Y 分量
            x * other.y - y * other.x   // Z 分量
        );
    }

    //计算点积
    float dot(const Vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vec3 operator+(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    Vec3 operator-(const Vec3& other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    Vec3 operator*(const float& n) const {
        return Vec3(x * n, y * n, z * n);
    }

    
};
