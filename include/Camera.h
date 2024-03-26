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

        void ProcessKeyboard(Direction direction, float deltaTime);
        void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
        void ProcessMouseScroll(float yoffset);
    private:
        Camera();

        float yaw              = -90.0f;
        float pitch            = 0.0f;
        float movementSpeed    = 2.5f;
        float mouseSensitivity = 0.1f;
        float zoom             = 45.0f;

        glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 worldUp  = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 right    = {};
        glm::vec3 front    = glm::vec3(0.0f, 0.0f, -1.0f);

        std::vector<Controllers::Event> m_eventQueue;
        std::array<bool, 6> m_movementDirectionVector = {false};

        void m_UpdateCameraVectors();
    };
}
