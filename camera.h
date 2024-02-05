#include <iostream>
#include <cmath>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera() : position(0.0f, 0.0f, 3.0f), front(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f),
        yaw(-90.0f), pitch(0.0f), speed(2.5f), sensitivity(0.1f), 
        zoom(1.0f), scrollSensitivity(0.5f), minZoom(0.2f), maxZoom(10.0f) {
        updateCameraVectors();
    }

    // Process keyboard input to move the camera
    void processKeyboard(float deltaTime) {
        float velocity = speed * deltaTime;
        if (keys[GLFW_KEY_W])
            position += front * velocity;
        if (keys[GLFW_KEY_S])
            position -= front * velocity;
        if (keys[GLFW_KEY_A])
            position -= right * velocity;
        if (keys[GLFW_KEY_D])
            position += right * velocity;
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
    glm::mat4 getViewMatrix() {
        return glm::lookAt(position, position + front, up);
    }

    float getZoom() {
        return zoom;
    }

    void updateKeys(int key, bool value) {
        keys[key] = value;
    }

private:
    // Camera attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

    // Euler angles
    float yaw;
    float pitch;

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
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(newFront);
        right = glm::normalize(glm::cross(front, up));
    }
};
