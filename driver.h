#include<iostream>
#include<vector>

#include "vec3.h"
#include "vec4.h"

class Driver {
public:
	std::string name;
	uint32_t node;
	std::string channel;//name of an animation channel; implies a data width (see below).
	std::vector<uint32_t> times;// array of numbers giving keyframe times.
	std::vector<uint32_t> values;//array of numbers giving keyframe values
	std::string interpolation = "LINEAR";// interpolation mode for the data (see below).
};
