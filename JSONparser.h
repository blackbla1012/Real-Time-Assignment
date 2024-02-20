#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <stdexcept>

#include "meshReader.h"
#include "scene.h"

class JsonParser {
public:
	JsonParser(const std::string& jsonFilePath) : jsonFilePath(jsonFilePath) {}
	
	uint32_t index = 0;
	std::map<uint32_t, Camera> cameras;
	std::map<uint32_t, Mesh> meshes;
	std::map<uint32_t, std::shared_ptr<Node>> nodes;
	std::vector<Driver> drivers;
	std::vector<Data> datas;
	std::vector<Scene> scenes;

	bool parse() {
		std::ifstream file(jsonFilePath);
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

				//parse camera object
				if (type == "\"type\":\"CAMERA\",") {
					std::cout << "camera detected!\n" << std::endl;

					Camera camera;
					std::string line;

					while (std::getline(ss,line)&& line !="},") {
			
						std::stringstream lineStream(line);
						std::string field;
						std::string value = "";

						if (std::getline(lineStream, field, ':')) {
							field = field.substr(field.find_first_not_of(" \t\r\n"), field.find_last_not_of(" \t\r\n") + 1);

							if (field == "\"name\"") {
								std::getline(lineStream, camera.name, ',');
								camera.name = camera.name.substr(camera.name.find_first_not_of(" \t\r\n\""), camera.name.find_last_not_of(" \t\r\n\""));
							}

							else if (field == "\"perspective\"") {
								continue;
							}
							else if (field == "\"aspect\"") {
								std::getline(lineStream, value, ',');
								std::istringstream(value) >> camera.aspect;
							}
							else if (field == "\"vfov\"") {
								std::getline(lineStream, value, ',');
								std::istringstream(value) >> camera.vfov;
							}
							else if (field == "\"near\"") {
								std::getline(lineStream, value, ',');
								std::istringstream(value) >> camera.nearC;
							}
							else if (field == "\"far\"") {
								std::getline(lineStream, value, ',');
								float far;
								std::istringstream(value) >> far;
								camera.farC = far;
							}

						}
						
					}
					index++;
					camera.s72Index = index;

					cameras.insert({ index, camera });
					std::cout << "object index: " << camera.s72Index << "\n" << std::endl;
					std::cout << "camera name: " << camera.name << "\n" << std::endl;
					std::cout << "camera aspect: " << camera.aspect << "\n" << std::endl;
					std::cout << "camera vfov: " << camera.vfov << "\n" << std::endl;
					std::cout << "camera near: " << camera.nearC << "\n" << std::endl;
					std::cout << "camera far: " << camera.farC << "\n" << std::endl;
		
				}

				//parse node object
				if (type == "\"type\":\"NODE\",") {
					std::cout << "node detected!\n" << std::endl;

					auto node = std::make_shared<Node>();
					std::string line;

					while (std::getline(ss, line) && line != "},") {

						std::stringstream lineStream(line);
						std::string field;
						std::string value = "";

						if (std::getline(lineStream, field, ':')) {
							field = field.substr(field.find_first_not_of(" \t\r\n"), field.find_last_not_of(" \t\r\n") + 1);

							if (field == "\"name\"") {
								std::getline(lineStream, node->name, ',');
								node->name = node->name.substr(node->name.find_first_not_of(" \t\r\n\""), node->name.find_last_not_of(" \t\r\n\""));
							}

							else if (field == "\"translation\"") {
								std::getline(lineStream, value, ',');
								if (value == "[]") continue;

								//read translation.x
								value = value.substr(value.find_first_not_of(" \t\r\n\"["), value.find_last_not_of(" \t\r\n\"]") + 1);
								node->translation.x = std::stof(value);

								//read translation.y
								std::getline(lineStream, value, ',');
								value = value.substr(value.find_first_not_of(" \t\r\n\"["), value.find_last_not_of(" \t\r\n\"]") + 1);
								node->translation.y = std::stof(value);

								//read translation.z
								std::getline(lineStream, value, ',');
								value = value.substr(value.find_first_not_of(" \t\r\n\"["), value.find_last_not_of(" \t\r\n\"]") + 1);
								node->translation.z = std::stof(value);

								std::cout << "node translation: " << node->translation.x << ", " << node->translation.y << ", " << node->translation.z << "\n" << std::endl;
							}

							else if (field == "\"rotation\"") {
								std::getline(lineStream, value, ',');
								if (value == "[]") continue;

								//read rotation.x
								value = value.substr(value.find_first_not_of(" \t\r\n\"["), value.find_last_not_of(" \t\r\n\"]") + 1);
								node->rotation.x = std::stof(value);

								//read rotation.y
								std::getline(lineStream, value, ',');
								value = value.substr(value.find_first_not_of(" \t\r\n\"["), value.find_last_not_of(" \t\r\n\"]") + 1);
								node->rotation.y = std::stof(value);

								//read rotation.z
								std::getline(lineStream, value, ',');
								value = value.substr(value.find_first_not_of(" \t\r\n\"["), value.find_last_not_of(" \t\r\n\"]") + 1);
								node->rotation.z = std::stof(value);

								//read rotation.w
								std::getline(lineStream, value, ',');
								value = value.substr(value.find_first_not_of(" \t\r\n\"["), value.find_last_not_of(" \t\r\n\"]") + 1);
								node->rotation.w = std::stof(value);

								std::cout << "node rotation: " << node->rotation.x << ", " << node->rotation.y << ", " << node->rotation.z << ", " << node->rotation.w << "\n" << std::endl;
							}
							else if (field == "\"scale\"") {
								std::getline(lineStream, value, ',');
								if (value == "[]") continue;

								//read scale.x
								value = value.substr(value.find_first_not_of(" \t\r\n\"["), value.find_last_not_of(" \t\r\n\"]") + 1);
								node->scale.x = std::stof(value);

								//read scale.y
								std::getline(lineStream, value, ',');
								value = value.substr(value.find_first_not_of(" \t\r\n\"["), value.find_last_not_of(" \t\r\n\"]") + 1);
								node->scale.y = std::stof(value);

								//read scale.z
								std::getline(lineStream, value, ',');
								value = value.substr(value.find_first_not_of(" \t\r\n\"["), value.find_last_not_of(" \t\r\n\"]") + 1);
								node->scale.z = std::stof(value);

								std::cout << "node scale: " << node->scale.x << ", " << node->scale.y << ", " << node->scale.z << "\n" << std::endl;
							}
							else if (field == "\"mesh\"") {
								std::getline(lineStream, value, ',');
								int meshIndex = std::stoi(value);
								node->mesh = static_cast<uint32_t>(meshIndex);
								std::cout << "node mesh: " << *node->mesh << "\n" << std::endl;
							}
							else if (field == "\"camera\"") {
								std::getline(lineStream, value, ',');
								int cameraIndex = std::stoi(value);
								node->camera = static_cast<uint32_t>(cameraIndex);
								std::cout << "node camera: " << *node->camera << "\n" << std::endl;

							}
							else if (field == "\"children\"") {
								while (std::getline(lineStream, value, ',')) {
									value = value.substr(value.find_first_not_of(" \t\r\n\"["), value.find_last_not_of(" \t\r\n\"]") + 1);
									int childStr = std::stoi(value);
									node->childrenIndex.push_back(static_cast<uint32_t>(childStr));
									std::cout << "node children: " << childStr << "\n" << std::endl;
								}
							}
						}
					}

					index++;
					node->s72Index = index;

					nodes.insert({ index, node });
					std::cout << "object index: " << node->s72Index << "\n" << std::endl;
					std::cout << "node name: " << node->name << "\n" << std::endl;
					//std::cout << "node translation: " << node.translation.x << "\n" << std::endl;
				}

				if (type == "\"type\":\"MESH\",") {
					std::cout << "mesh detected!\n" << std::endl;

					Mesh mesh;
					std::string line;

					while (std::getline(ss, line) && line != "},") {

						std::stringstream lineStream(line);
						std::string field;
						std::string value = "";

						if (std::getline(lineStream, field, ':')) {
							//È¥³ý×Ö¶ÎÃûÁ½¶ËµÄ¿Õ°××Ö·û
							field = field.substr(field.find_first_not_of(" \t\r\n"), field.find_last_not_of(" \t\r\n") + 1);

							if (field == "\"name\"") {
								std::getline(lineStream, mesh.name, ',');
								//È¥³ý×Ö¶ÎÃûÁ½¶ËµÄ¿Õ°××Ö·ûºÍË«ÒýºÅ
								mesh.name = mesh.name.substr(mesh.name.find_first_not_of(" \t\r\n\""), mesh.name.find_last_not_of(" \t\r\n\""));
							}

							else if (field == "\"topology\"") {
							}

							else if (field == "\"count\"") {
								std::getline(lineStream, value, ',');
								int vertCount = std::stoi(value);
								mesh.count = static_cast<uint32_t>(vertCount);
								std::cout << "mesh count: " << mesh.count << "\n" << std::endl;
							}
							else if (field == "\"attributes\"") {
								continue;
							}
							else if (field == "\"POSITION\"") {
								std::getline(lineStream, field, ':');//read "src"
								std::getline(lineStream, value, ',');
								value = value.substr(value.find_first_not_of(" \t\r\n\""), value.find_last_not_of(" \t\r\n\""));
								mesh.POSITION.src = value;
								std::cout << "mesh src: " << mesh.POSITION.src << std::endl;

								std::getline(lineStream, field, ':');//read "offset"
								std::getline(lineStream, value, ',');
								value = value.substr(value.find_first_not_of(" \t\r\n\""), value.find_last_not_of(" \t\r\n\"") + 1);
								int offset = std::stoi(value);
								mesh.POSITION.offset = static_cast<uint32_t>(offset);
								std::cout << "mesh offset: " << mesh.POSITION.offset << std::endl;

								std::getline(lineStream, field, ':');//read "stride"
								std::getline(lineStream, value, ',');
								value = value.substr(value.find_first_not_of(" \t\r\n\""), value.find_last_not_of(" \t\r\n\"") + 1);
								int stride = std::stoi(value);
								mesh.POSITION.stride = static_cast<uint32_t>(stride);
								std::cout << "mesh stride: " << mesh.POSITION.stride << std::endl;

								std::getline(lineStream, field, ':');//read "format"
								std::getline(lineStream, value, '}');
								value = value.substr(value.find_first_not_of(" \t\r\n\""), value.find_last_not_of(" \t\r\n\"}"));
								mesh.POSITION.format = value;
								std::cout << "mesh format: " << mesh.POSITION.format << std::endl;
							}
							else if (field == "\"NORMAL\"") {
								std::getline(lineStream, field, ':');//read "src"
								std::getline(lineStream, value, ',');
								value = value.substr(value.find_first_not_of(" \t\r\n\""), value.find_last_not_of(" \t\r\n\""));
								mesh.NORMAL.src = value;
								std::cout << "mesh src: " << mesh.NORMAL.src << std::endl;

								std::getline(lineStream, field, ':');//read "offset"
								std::getline(lineStream, value, ',');
								value = value.substr(value.find_first_not_of(" \t\r\n\""), value.find_last_not_of(" \t\r\n\"") + 1);
								int offset = std::stoi(value);
								mesh.NORMAL.offset = static_cast<uint32_t>(offset);
								std::cout << "mesh offset: " << mesh.NORMAL.offset << std::endl;

								std::getline(lineStream, field, ':');//read "stride"
								std::getline(lineStream, value, ',');
								value = value.substr(value.find_first_not_of(" \t\r\n\""), value.find_last_not_of(" \t\r\n\"") + 1);
								int stride = std::stoi(value);
								mesh.NORMAL.stride = static_cast<uint32_t>(stride);
								std::cout << "mesh stride: " << mesh.NORMAL.stride << std::endl;

								std::getline(lineStream, field, ':');//read "format"
								std::getline(lineStream, value, '}');
								value = value.substr(value.find_first_not_of(" \t\r\n\""), value.find_last_not_of(" \t\r\n\"}"));
								mesh.NORMAL.format = value;
								std::cout << "mesh format: " << mesh.NORMAL.format << std::endl;
							}
							else if (field == "\"COLOR\"") {
								std::getline(lineStream, field, ':');//read "src"
								std::getline(lineStream, value, ',');
								value = value.substr(value.find_first_not_of(" \t\r\n\""), value.find_last_not_of(" \t\r\n\""));
								mesh.COLOR.src = value;
								std::cout << "mesh src: " << mesh.COLOR.src << std::endl;

								std::getline(lineStream, field, ':');//read "offset"
								std::getline(lineStream, value, ',');
								value = value.substr(value.find_first_not_of(" \t\r\n\""), value.find_last_not_of(" \t\r\n\"") + 1);
								int offset = std::stoi(value);
								mesh.COLOR.offset = static_cast<uint32_t>(offset);
								std::cout << "mesh offset: " << mesh.COLOR.offset << std::endl;

								std::getline(lineStream, field, ':');//read "stride"
								std::getline(lineStream, value, ',');
								value = value.substr(value.find_first_not_of(" \t\r\n\""), value.find_last_not_of(" \t\r\n\"") + 1);
								int stride = std::stoi(value);
								mesh.COLOR.stride = static_cast<uint32_t>(stride);
								std::cout << "mesh stride: " << mesh.COLOR.stride << std::endl;

								std::getline(lineStream, field, ':');//read "format"
								std::getline(lineStream, value, '}');
								value = value.substr(value.find_first_not_of(" \t\r\n\""), value.find_last_not_of(" \t\r\n\"}"));
								mesh.COLOR.format = value;
								std::cout << "mesh format: " << mesh.COLOR.format << std::endl;
							}
						}
					}

					index++;
					mesh.s72Index = index;

					meshes.insert({ index, mesh });
					std::cout << "object index: " << mesh.s72Index << "\n" << std::endl;
					std::cout << "mesh name: " << mesh.name << "\n" << std::endl;
				}

				if (type == "\"type\":\"DRIVER\",") {
					std::cout << "driver detected!\n" << std::endl;

					Driver driver;
					std::string line;

					while (std::getline(ss, line) && line != "},") {

						std::stringstream lineStream(line);
						std::string field;
						std::string value = "";

						if (std::getline(lineStream, field, ':')) {
							//È¥³ý×Ö¶ÎÃûÁ½¶ËµÄ¿Õ°××Ö·û
							field = field.substr(field.find_first_not_of(" \t\r\n"), field.find_last_not_of(" \t\r\n") + 1);

							if (field == "\"name\"") {
								std::getline(lineStream, driver.name, ',');
								//È¥³ý×Ö¶ÎÃûÁ½¶ËµÄ¿Õ°××Ö·ûºÍË«ÒýºÅ
								driver.name = driver.name.substr(driver.name.find_first_not_of(" \t\r\n\""), driver.name.find_last_not_of(" \t\r\n\""));
							}

							else if (field == "\"node\"") {
								std::getline(lineStream, value, ',');
								int nodeIndex = std::stoi(value);
								driver.node = static_cast<uint32_t>(nodeIndex);
								std::cout << "driver node: " << driver.node << "\n" << std::endl;
							}

							else if (field == "\"channel\"") {
								std::getline(lineStream, driver.channel, ',');
								driver.channel = driver.channel.substr(driver.channel.find_first_not_of(" \t\r\n\""), driver.channel.find_last_not_of(" \t\r\n\""));
								std::cout << "driver channel: " << driver.channel << "\n" << std::endl;
							}
							else if (field == "\"times\"") {
								while (std::getline(lineStream, value, ',')) {
									value = value.substr(value.find_first_not_of(" \t\r\n\"["), value.find_last_not_of(" \t\r\n\"]") + 1);
									driver.times.push_back(std::stof(value));
									//std::cout << "driver times: " << *(driver.times.end()-1) << "\n" << std::endl;
								}
							}
							else if (field == "\"values\"") {
								while (std::getline(lineStream, value, ',')) {
									value = value.substr(value.find_first_not_of(" \t\r\n\"["), value.find_last_not_of(" \t\r\n\"]") + 1);
									driver.values.push_back(std::stof(value));
									//std::cout << "driver values: " << *(driver.values.end() - 1) << "\n" << std::endl;
								}
							}
							else if (field == "\"interpolation\"") {
								std::getline(lineStream, driver.interpolation, ',');
								driver.interpolation = driver.interpolation.substr(driver.interpolation.find_first_not_of(" \t\r\n\""), driver.interpolation.find_last_not_of(" \t\r\n\""));
								std::cout << "driver interpolation: " << driver.interpolation << "\n" << std::endl;
							}
						}
					}

					index++;
					driver.s72Index = index;
					std::cout << "object index: " << driver.s72Index << "\n" << std::endl;
					std::cout << "driver name: " << driver.name << "\n" << std::endl;

				}

				if (type == "\"type\":\"SCENE\",") {
					std::cout << "scene detected!\n" << std::endl;

					Scene scene;
					std::string line;

					while (std::getline(ss, line) && line != "},") {

						std::stringstream lineStream(line);
						std::string field;
						std::string value = "";

						if (std::getline(lineStream, field, ':')) {
							//È¥³ý×Ö¶ÎÃûÁ½¶ËµÄ¿Õ°××Ö·û
							field = field.substr(field.find_first_not_of(" \t\r\n"), field.find_last_not_of(" \t\r\n") + 1);

							if (field == "\"name\"") {
								std::getline(lineStream, scene.name, ',');
								//È¥³ý×Ö¶ÎÃûÁ½¶ËµÄ¿Õ°××Ö·ûºÍË«ÒýºÅ
								scene.name = scene.name.substr(scene.name.find_first_not_of(" \t\r\n\""), scene.name.find_last_not_of(" \t\r\n\""));
							}

							else if (field == "\"roots\"") {
								while (std::getline(lineStream, value, ',')) {
									value = value.substr(value.find_first_not_of(" \t\r\n\"["), value.find_last_not_of(" \t\r\n\"]") + 1);
									int root = std::stoi(value);
									scene.roots.push_back(static_cast<uint32_t>(root));
									std::cout << "scene root: " << scene.roots.back() << "\n" << std::endl;
								}
							}
						}
					}

					index++;
					scene.s72Index = index;
					std::cout << "object index: " << scene.s72Index << "\n" << std::endl;
					std::cout << "scene name: " << scene.name << "\n" << std::endl;
				}
			}
		}

		return true;
	}
};

