#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Render {
    enum Direction {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    class Camera {
    public:
        static Camera& GetInstance() {
            static Camera instance;
            return instance;
        }

        [[nodiscard]] glm::mat4 GetViewMatrix() const;

        void ProcessKeyboard(Direction direction, float deltaTime);
        void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
        void ProcessMouseScroll(float yoffset);

        Camera(const Camera&) = delete;
        Camera(Camera&&) = delete;
        Camera& operator=(const Camera&) = delete;
        Camera& operator=(Camera&&) = delete;
    private:
        float yaw              = -90.f;
        float pitch            = 0.f;
        float movementSpeed    = 2.5f;
        float mouseSensitivity = 0.1f;
        float zoom             = 45.f;

        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 worldUp;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 front = glm::vec3(0, 0, -1);

        Camera() {
            m_UpdateCameraVectors();
        }

        void m_UpdateCameraVectors();
    };
}
