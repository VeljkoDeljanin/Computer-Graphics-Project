#include "Camera.h"

glm::mat4 Render::Camera::GetViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void Render::Camera::ProcessKeyboard(Render::Direction direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    switch (direction) {
        case FORWARD:
            position += front * velocity;
            break;
        case BACKWARD:
            position -= front * velocity;
            break;
        case RIGHT:
            position += right * velocity;
            break;
        case LEFT:
            position -= right * velocity;
            break;
        case UP:
            position += worldUp * velocity;
            break;
        case DOWN:
            position -= worldUp * velocity;
            break;
    }
}

void Render::Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (constrainPitch) {
        if (pitch > 89.f)
            pitch = 89.f;
        if (pitch < -89.f)
            pitch = -89.f;
    }

    m_UpdateCameraVectors();
}

void Render::Camera::ProcessMouseScroll(float yoffset) {
    zoom -= yoffset;

    if (zoom < 1.f)
        zoom = 1.f;
    if (zoom > 45.f)
        zoom = 45.f;
}

void Render::Camera::m_UpdateCameraVectors() {
    glm::vec3 newFront;
    newFront.x = static_cast<float>(cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
    newFront.y = static_cast<float>(sin(glm::radians(pitch)));
    newFront.z = static_cast<float>(sin(glm::radians(yaw)) * cos(glm::radians(pitch)));

    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up    = glm::normalize(glm::cross(right, front));
}
