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
    return glm::lookAt(Position, Position + Front, Up);
}

float Render::Camera::GetZoom() const {
    return Zoom;
}

glm::vec3 Render::Camera::GetPosition() const {
    return Position;
}

void Render::Camera::ProcessKeyboard(Render::Direction direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    switch (direction) {
        case Render::Direction::FORWARD:
            Position += Front * velocity;
            break;
        case Render::Direction::BACKWARD:
            Position -= Front * velocity;
            break;
        case Render::Direction::RIGHT:
            Position += Right * velocity;
            break;
        case Render::Direction::LEFT:
            Position -= Right * velocity;
            break;
        case Render::Direction::UP:
            Position += WorldUp * velocity;
            break;
        case Render::Direction::DOWN:
            Position -= WorldUp * velocity;
            break;
    }

//    Position.y = 1.0f;
}

void Render::Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    if (constrainPitch) {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    m_UpdateCameraVectors();
}

void Render::Camera::ProcessMouseScroll(float yoffset) {
    Zoom -= yoffset;

    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

Render::Camera::Camera() : Position(glm::vec3(0.0f, 1.0f, -11.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
                           Front(glm::vec3(0.0f, 0.0f, 1.0f)) {
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
    glm::vec3 front;
    front.x = static_cast<float>(cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)));
    front.y = static_cast<float>(sin(glm::radians(Pitch)));
    front.z = static_cast<float>(sin(glm::radians(Yaw)) * cos(glm::radians(Pitch)));

    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up    = glm::normalize(glm::cross(Right, Front));
}

float Render::Camera::GetYaw() const {
    return Yaw;
}

float Render::Camera::GetPitch() const {
    return Pitch;
}

glm::vec3 Render::Camera::GetFront() const {
    return Front;
}
