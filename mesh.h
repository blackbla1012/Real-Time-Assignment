#include<iostream>
#include<vector>

#include "vec3.h"
#include "vec4.h"

struct data {
	std::string src;
	uint32_t offset;
	uint32_t stride;
	std::string format;
};

class Mesh {
public:
	std::string type;
	std::string name;
	std::string topology; //the primitive type and layout used in the mesh. Valid values are VkPrimitiveTopology identifiers without the prefix (e.g., "TRIANGLE_LIST").
	uint32_t count; //the number of vertices in the mesh
	
	struct indices {
		std::string src;
		uint32_t offset;
		std::string format;
	};//optional - if specified, a data stream containing indices for indexed drawing commands.
	
	struct attributes {
		data POSITION;
		data NORMAL;
		data COLOR;
	};
};
