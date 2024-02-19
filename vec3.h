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
};
