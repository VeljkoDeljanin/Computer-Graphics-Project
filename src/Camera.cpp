#include "Camera.h"

#include <GLFW/glfw3.h>

#include "Controllers/ServiceLocator.h"

void Render::Camera::Notify(Controllers::Event event) {
    m_eventQueue.push_back(event);
}

void Render::Camera::Update(float dt) {
    for (auto& event : m_eventQueue) {
        if (event.eventType == Controllers::EventType::MouseMoved) {
            ProcessMouseMovement(
                    static_cast<float>(event.mouseMoved.xoffset),
                    static_cast<float>(event.mouseMoved.yoffset)
            );
        }
        else if (event.eventType == Controllers::EventType::Keyboard) {
            auto keyboard = event.keyboard;
            switch (keyboard.key) {
                case GLFW_KEY_W:
                    m_movementDirectionVector[static_cast<int>(Render::Direction::FORWARD)]
                            = keyboard.keyState == Controllers::KeyState::Pressed;
                    break;
                case GLFW_KEY_S:
                    m_movementDirectionVector[static_cast<int>(Render::Direction::BACKWARD)]
                            = keyboard.keyState == Controllers::KeyState::Pressed;
                    break;
                case GLFW_KEY_A:
                    m_movementDirectionVector[static_cast<int>(Render::Direction::LEFT)]
                            = keyboard.keyState == Controllers::KeyState::Pressed;
                    break;
                case GLFW_KEY_D:
                    m_movementDirectionVector[static_cast<int>(Render::Direction::RIGHT)]
                            = keyboard.keyState == Controllers::KeyState::Pressed;
                    break;
                case GLFW_KEY_SPACE:
                    m_movementDirectionVector[static_cast<int>(Render::Direction::UP)]
                            = keyboard.keyState == Controllers::KeyState::Pressed;
                    break;
                case GLFW_KEY_LEFT_SHIFT:
                    m_movementDirectionVector[static_cast<int>(Render::Direction::DOWN)]
                            = keyboard.keyState == Controllers::KeyState::Pressed;
                    break;
            }
        }
        else if (event.eventType == Controllers::EventType::Scroll) {
            ProcessMouseScroll(static_cast<float>(event.scroll.yoffset));
        }
    }

    for (int direction = 0; direction < 6; direction++)
        if (m_movementDirectionVector[direction])
            ProcessKeyboard(static_cast<Direction>(direction), dt);

    m_eventQueue.clear();
}

glm::mat4 Render::Camera::GetViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

float Render::Camera::GetZoom() const {
    return zoom;
}

void Render::Camera::ProcessKeyboard(Render::Direction direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    switch (direction) {
        case Render::Direction::FORWARD:
            position += front * velocity;
            break;
        case Render::Direction::BACKWARD:
            position -= front * velocity;
            break;
        case Render::Direction::RIGHT:
            position += right * velocity;
            break;
        case Render::Direction::LEFT:
            position -= right * velocity;
            break;
        case Render::Direction::UP:
            position += worldUp * velocity;
            break;
        case Render::Direction::DOWN:
            position -= worldUp * velocity;
            break;
    }
}

void Render::Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw   += xoffset;
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

    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;
}

Render::Camera::Camera() {
    Controllers::ServiceLocator::GetInstance().GetEventController().SubscribeToEvent(
            Controllers::EventType::Keyboard, this
    );
    Controllers::ServiceLocator::GetInstance().GetEventController().SubscribeToEvent(
            Controllers::EventType::MouseMoved, this
    );
    Controllers::ServiceLocator::GetInstance().GetEventController().SubscribeToEvent(
            Controllers::EventType::Scroll, this
    );
    m_UpdateCameraVectors();
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
