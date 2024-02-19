#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "scene.h"

#include "vec3.h"
#include "vec4.h"

//vertex structure

class MeshReader {
public:
	MeshReader(const std::string& fileName, const uint32_t count) : fileName(fileName), count(count) {}

	std::vector<Vertex> vertices;
	
	bool readMesh(size_t offset, size_t stride, std::string format, std::string type) {
		std::ifstream file("JSON/" + fileName, std::ios::binary);
		if (!file.is_open()) {
			std::cerr << "Failed to open file!" << fileName << std::endl;
			return false;
		}

		//computer size of one vertex
		size_t elementSize = getElementSize(format);
		if (elementSize == 0) {
			std::cerr << "Unsupported format: " << format << std::endl;
			return false;
		}

		file.seekg(offset, std::ios::beg);
		if (type == "POSITION") {
			vertices.reserve(count);
			for (size_t i = 0; i < count; ++i) {
				Vertex vertex;
				file.read(reinterpret_cast<char*>(&vertex.POSITION), elementSize);
				vertices.push_back(vertex);
				file.seekg(stride - elementSize, std::ios::cur); // jump remaining data
			}
		}
		else if(type == "NORMAL") {
			for (size_t i = 0; i < count; ++i) {
				file.read(reinterpret_cast<char*>(&vertices[i].NORMAL), elementSize);
				file.seekg(stride - elementSize, std::ios::cur); // jump remaining data
			}
		}
		else if (type == "COLOR") {
			for (size_t i = 0; i < count; ++i) {
				file.read(reinterpret_cast<char*>(&vertices[i].COLOR), elementSize);
				file.seekg(stride - elementSize, std::ios::cur); // jump remaining data
			}
		}

		file.close();
		//这里只是把所有数据都放在了vector中

		return true;
	}


private:
	std::string fileName;
	uint32_t count;

	size_t getElementSize(const std::string& format) {
		if (format == "R32G32B32_SFLOAT") {
			return 3 * sizeof(float);
		}
		else if (format == "R8G8B8A8_UNORM") {
			return 4 * sizeof(uint8_t);
		}
		else {
			throw std::invalid_argument("Unsupported format: " + format);
			return 0;
		}
		return 0;
	}
};
