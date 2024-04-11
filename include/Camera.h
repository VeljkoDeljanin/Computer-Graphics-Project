#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Controllers/EventController.h"

namespace Render {
    enum class Direction {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    class Camera : public Controllers::Observer {
    public:
        Camera(const Camera&) = delete;
        Camera(Camera&&) = delete;
        Camera& operator=(const Camera&) = delete;
        Camera& operator=(Camera&&) = delete;

        static Camera& GetInstance() {
            static Camera instance;
            return instance;
        }

        void Notify(Controllers::Event event) override;
        void Update(float dt);

        [[nodiscard]] glm::mat4 GetViewMatrix() const;
        [[nodiscard]] float GetZoom() const;
        [[nodiscard]] glm::vec3 GetPosition() const;
        [[nodiscard]] float GetYaw() const;
        [[nodiscard]] float GetPitch() const;
        [[nodiscard]] glm::vec3 GetFront() const;
        [[nodiscard]] glm::vec3 GetUp() const;
        [[nodiscard]] glm::vec3 GetRight() const;

        void SetMovementSpeed(float movementSpeed);

        void ProcessKeyboard(Direction direction, float deltaTime);
        void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
        void ProcessMouseScroll(float yoffset);
    private:
        Camera();

        float Yaw              = -270.0f;
        float Pitch            = 0.0f;
        float MovementSpeed;
        float MouseSensitivity = 0.1f;
        float Zoom             = 45.0f;

        glm::vec3 Position;
        glm::vec3 WorldUp;
        glm::vec3 Up{};
        glm::vec3 Right{};
        glm::vec3 Front;

        std::vector<Controllers::Event> m_eventQueue;
        std::array<bool, 6> m_movementDirectionVector = {false};

        void m_UpdateCameraVectors();
    };
}
