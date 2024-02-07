#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <stdexcept>

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
	JsonParser(const std::string& jsonFilePath) : jsonFilePath(jsonFilePath){}
	

private:
	std::string jsonFilePath;

	bool startParsing(const std::string& jsonFilePath, const std::string& expectedHeader) {
		std::ifstream file(jsonFilePath);
		if (!file.is_open()) {
			throw std::runtime_error("failed to open the file!");
			return false;
		}

		std::string header;
		if (!(file >> header)) {
			throw std::runtime_error("failed to read the file header!");
			return false;
		}

		if (header != expectedHeader) {
			throw std::runtime_error("file header does not match expected value!");
			return false;
		}

		std::cout << "File header matches expected value." << std::endl;


		return true;
	}

	bool parseJson(const std::string& jsonString) {
		size_t index = 0;
		return parseValue;
	}

	bool parseValue(const std::string& jsonString, size_t& index, JsonValue& value) {
		skipWhitespace(jsonString, index);

		char currentChar = jsonString[index];

	}

	bool parseObject(const std::string& jasonString, size_t& index, std::map<std::string, JsonValue>) {

	}

	bool parseArray() {

	}

	bool parseString() {

	}

	bool parseNumber(const std::string& jsonString, size_t& index, double& num) {
		size_t start = index;
		while(isdigit(jsonString[index]) || jsonString[index])
	}

	void skipWhitespace(const std::string& jsonString, size_t& index) {
		while (index < jsonString.size() && isspace(jsonString[index])) {
			index++;
		}
	}
};