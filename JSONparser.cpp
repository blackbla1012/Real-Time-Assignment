#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <stdexcept>

#include "camera.h"
#include "mesh.h"
#include "node.h"
#include "scene.h"
#include "data.h"
#include "driver.h"

enum class JsonValueType {
	OBJECT,
	ARRAY,
	STRING,
	NUMBER
};

struct JsonValue {
	//JsonValueType type;
	std::string type;
	std::string name;
	//later sort into different h struct files

};

class JsonParser {
public:
	JsonParser(const std::string& jsonFilePath) : jsonFilePath(jsonFilePath) {}

	bool parse() {
		std::ifstream file("JSON/sg-Articulation.s72");
		if (!file.is_open()) {
			throw std::runtime_error("Failed to open the file!");
			return false;
		}

		std::stringstream buffer;
		buffer << file.rdbuf();
		std::string jsonString = buffer.str();

		if (!parseJson(jsonString)) {
			std::cout << "failed to start parsing!" << std::endl;
		}
		
		return true;
	}


private:
	std::string jsonFilePath;

	bool parseJson(const std::string& jsonString) {
		std::stringstream ss(jsonString);
		std::string header;
		if (!(ss >> header)) {
			throw std::runtime_error("failed to read the file header!");
			return false;
		}

		if (header != "[\"s72-v1\",") {
			throw std::runtime_error("file header does not match expected value!");
			return false;
		}

		std::cout << "File header matches expected value." << std::endl;

		//parse each JSON object
		while (ss >> header) {
			if (header == "{") {
				std::string type;
				ss >> type;

				std::cout << type << std::endl;

				
				if (type == "\"type\":\"CAMERA\",") {
					std::cout << "camera detected!\n" << std::endl;

					Camera camera;
					std::string field;
					while (ss >> field && field != "}") {
						if (field == "\"name\":") {
							ss >> camera.name;
							std::cout << "camera name detected!\n" << std::endl;
						}
						else if (field == "\"perspective\":") {
							std::cout << "camera perspective detected!\n" << std::endl;
							//ss >> header;

						}
					}
				}

				if (type == "\"type\":\"NODE\",") {
					std::cout << "node detected!\n" << std::endl;
				}

				if (type == "\"type\":\"MESH\",") {
					std::cout << "mesh detected!\n" << std::endl;
				}
				
				if (type == "\"type\":\"DRIVER\",") {
					std::cout << "driver detected!\n" << std::endl;
				}

				if (type == "\"type\":\"SCENE\",") {
					std::cout << "scene detected!\n" << std::endl;
				}
			}
		}

		return true;
	}

	bool parseObject(const std::string& jasonString, size_t& index, std::map<std::string, JsonValue>) {

	}

	bool parseArray() {

	}

	bool parseString() {

	}

	bool parseNumber(const std::string& jsonString, size_t& index, double& num) {
	}

	void skipWhitespace(const std::string& jsonString, size_t& index) {
		while (index < jsonString.size() && isspace(jsonString[index])) {
			index++;
		}
	}
};

int main() {
	JsonParser jsonParser("JSON/sg-Articulation.s72");

	if (jsonParser.parse()) {
		std::cout << "Succeeded to parse the file!" << std::endl;
	}
	else {
		std::cout << "Failed to parse the file!" << std::endl;
	}

	return 0;
}