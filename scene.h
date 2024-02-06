#include<iostream>
#include<vector>

class Scene {
public:
	std::string type;
	std::string name;
	std::vector<uint32_t> roots;//array of references to nodes at which to start drawing the scene.
};
