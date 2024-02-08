#include<iostream>
#include<vector>

#include "vec3.h"
#include "vec4.h"

class Node {
public:
	std::string name;
	Vec3 translation;
	Vec4 rotation;
	Vec3 scale;
	std::vector<uint32_t> children;
	int camera;
	int mesh;
};
