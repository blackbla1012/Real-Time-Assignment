#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <map>
#include <optional>
#include <array>
#include <functional>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "vec3.h"
#include "vec4.h"
#include "mat4.h"

struct Vertex {
    Vec3 POSITION;
    Vec3 NORMAL;
    Vec4 COLOR;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;//move data entry after each vertex

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};//[0]->POSITION, [1]-> NORMAL, [2]-> COLOR
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, POSITION);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;//this means vec3
        attributeDescriptions[1].offset = offsetof(Vertex, NORMAL);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R8G8B8A8_UNORM;
        attributeDescriptions[2].offset = offsetof(Vertex, COLOR);

        return attributeDescriptions;
    }
};

class Scene {
public:
	std::uint32_t s72Index;
	std::string name;
	std::vector<uint32_t> roots;//array of references to nodes at which to start drawing the scene.
};

class Mesh {
public:
	std::uint32_t s72Index;
	std::string name;
	std::string topology; //the primitive type and layout used in the mesh. Valid values are VkPrimitiveTopology identifiers without the prefix (e.g., "TRIANGLE_LIST").
	uint32_t count; //the number of vertices in the mesh
	/*
	struct indices {
		std::string src;
		uint32_t offset;
		std::string format;
	};//optional - if specified, a data stream containing indices for indexed drawing commands.
	*/

	struct data {
		std::string src;
		uint32_t offset;
		uint32_t stride;
		std::string format;
	};

	data POSITION;
	data NORMAL;
	data COLOR;

    std::vector<Vertex> vertices;
};

class Node : public std::enable_shared_from_this<Node> {
public:
	std::uint32_t s72Index;
	std::string name;
	Vec3 translation = Vec3();
	Vec4 rotation = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
	Vec3 scale = Vec3(1.0f, 1.0f, 1.0f);
	std::optional<uint32_t> camera;
    std::optional<uint32_t> mesh;
    std::vector<uint32_t> childrenIndex;
    std::vector<std::shared_ptr<Node>> children;
    std::shared_ptr<Node> parent; // ʹ�� std::shared_ptr<Node> ���� Node*
    Mat4 localToWorld;

    bool operator==(const Node& other) const {
        return s72Index == other.s72Index;
    }

};

class Driver {
public:
	std::uint32_t s72Index;
	std::string name;
	uint32_t node;
	std::string channel;//name of an animation channel; implies a data width (see below).
	std::vector<float> times;// array of numbers giving keyframe times.
	std::vector<float> values;//array of numbers giving keyframe values
	std::string interpolation = "LINEAR";// interpolation mode for the data (see below).
};

class Data {
public:
	std::uint32_t s72Index;
	std::string name;
	std::vector<uint32_t> data;
};

class Camera {
public:
    Camera() : position(0.0f, 0.0f, 0.0f), front(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f),
        yaw(0.0f), pitch(0.0f), speed(2.5f), sensitivity(0.1f),
        zoom(1.0f), scrollSensitivity(0.5f), minZoom(0.2f), maxZoom(10.0f) {
        updateCameraVectors();
    }//yaw - angle that define the rotation(around axes Y) pitch - define rotation up/down(around axes X)

    std::uint32_t s72Index;
    std::string name;
    float aspect;
    float vfov;
    float nearC;
    float farC = 100.0f;
    
    Vec3 position;
    float yaw;
    float pitch;

    // Camera attributes
    Vec3 front;
    Vec3 up;
    Vec3 right;

    // Process keyboard input to move the camera
    void processKeyboard(float deltaTime) {
        float velocity = speed * deltaTime;
        if (keys[GLFW_KEY_W])
            position = position - up * velocity;
        if (keys[GLFW_KEY_S])
            position = position + up * velocity;
        if (keys[GLFW_KEY_A])
            position = position + right * velocity;
        if (keys[GLFW_KEY_D])
            position = position - right * velocity;
    }

    // Process mouse input to look around
    void processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true) {
        xOffset *= sensitivity;
        yOffset *= sensitivity;

        yaw += xOffset;
        pitch += yOffset;

        // Constrain pitch to avoid flipping
        if (constrainPitch) {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        updateCameraVectors();
    }

    void processMouseScroll(float yOffset) {
        zoom -= yOffset * scrollSensitivity;

        if (zoom < minZoom) zoom = minZoom;
        if (zoom > maxZoom) zoom = maxZoom;
    }

    // Get the view matrix
    Mat4 getViewMatrix() {
        return Mat4::lookAt(position, position + front, up);
    }

    float getZoom() {
        return zoom;
    }

    void updateKeys(int key, bool value) {
        keys[key] = value;
    }

private:
    // Camera parameters
    float speed;
    float sensitivity;

    // Key states
    std::map<int, bool> keys;

    //scroll zoom parameters
    float zoom;
    float scrollSensitivity;
    float minZoom;
    float maxZoom;

    // Update the camera's vectors based on Euler angles
    void updateCameraVectors() {
        Vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = newFront.normalize();
        right = front.cross(up).normalize();
    }
};

class BBox {
public:
    Vec3 min = Vec3(std::numeric_limits<float>::max());
    Vec3 max = Vec3(std::numeric_limits<float>::lowest());

    void update(const Vec3& point) {
        min = Vec3::min(min, point);
        max = Vec3::max(max, point);
    }

};
