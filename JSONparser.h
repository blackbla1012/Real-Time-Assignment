#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

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
	

private:
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