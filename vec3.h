#ifndef VEC3_H
#define VEC3_H

struct Vec3 {
	float x;
	float y;
	float z;

	//Constructors
	Vec3() : x(0.0f), y(0.0f), z(0.0f){}
	Vec3(float x, float y, float z) : x(x), y(y), z(z){}
	Vec3(float x) : x(x), y(x), z(x){}
};

#endif // !VEC3_H
