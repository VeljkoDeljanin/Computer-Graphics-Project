#pragma once

#include <unordered_map>

struct GLFWwindow;

namespace Controllers {
    enum class KeyState {
        Released,
        JustPressed,
        Pressed,
        JustReleased
    };

    struct MouseMovementData {
        double lastX = 0.0f;
        double lastY = 0.0f;
        double currentX = 0.0f;
        double currentY = 0.0f;
        bool firstMouse = true;
    };

    struct ScrollMovementData {
        double lastY = 0.f;
        double currentY = 0.f;
    };

    class InputController {
    public:
        InputController(const InputController&) = delete;
        InputController(InputController&&) = delete;
        InputController& operator=(const InputController&) = delete;
        InputController& operator=(InputController&&) = delete;

        static InputController& GetInstance() {
            static InputController instance;
            return instance;
        }

        void Update();

        Controllers::KeyState GetKeyState(int key) const;

        void ProcessKeyCallback(GLFWwindow *window, int key, int action);
        void ProcessMouseMovementCallback(double xpos, double ypos);
        void ProcessMouseScrollCallback(double ypos);
    private:
        InputController() {
            m_keys.reserve(1024);
        }

        struct KeyFrameState {
            KeyState keyState{KeyState::Released};
            int glfwKeyAction = -1;
        };

        std::unordered_map<int, KeyFrameState> m_keys;
        MouseMovementData m_mouse;
        ScrollMovementData m_scroll;
    };

    const char *ToString(KeyState state);
}
