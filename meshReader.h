#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "scene.h"

//vertex structure

class MeshReader {
public:
	MeshReader(const std::string& fileName) : fileName(fileName) {}

	std::vector<Vertex> vertices;

	bool readMesh(size_t offset, size_t stride, std::string format, uint32_t count, std::string type) {
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
			//vertices.reserve(Count);
			for (size_t i = 0; i < count; ++i) {
				Vertex vertex;
				file.read(reinterpret_cast<char*>(&vertex), elementSize);
				vertices.push_back(vertex);
				file.seekg(stride - elementSize, std::ios::cur); // jump remaining data
			}
		}
		else if(type == "NORMAL") {
			//normals.reserve(Count);
			for (size_t i = 0; i < count; ++i) {
				Normal normal;
				file.read(reinterpret_cast<char*>(&normal), elementSize);
				normals.push_back(normal);
				file.seekg(stride - elementSize, std::ios::cur); // jump remaining data
			}
		}
		else if (type == "COLOR") {
			//colors.reserve(Count);
			for (size_t i = 0; i < count; ++i) {
				Color color;
				file.read(reinterpret_cast<char*>(&color), elementSize);
				colors.push_back(color);
				file.seekg(stride - elementSize, std::ios::cur); // jump remaining data
			}
		}

		file.close();
		//这里只是把所有数据都放在了vector中

		return true;
	}


private:
	std::string fileName;

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
