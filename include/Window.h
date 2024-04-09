#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Controllers/EventController.h"

namespace Render {
    enum class Keys {
        KEY_Q,
        KEY_E,
        KEY_ARROW_UP,
        KEY_ARROW_DOWN
    };

    class Window : public Controllers::Observer {
    public:
        Window(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&) = delete;

        static Window& GetInstance() {
            static Window instance;
            return instance;
        }

        void Notify(Controllers::Event event) override;

        void Init();
        [[nodiscard]] bool IsRunning() const;
        void Update();
        void Destroy();

        static GLFWwindow *GetGlfwWindow();

        static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    private:
        Window();

        static inline GLFWwindow *m_glfwWindow = nullptr;
        bool m_running = false;
        std::vector<Controllers::Event> m_eventQueue;
        std::array<bool, 4> m_keysPressedDown = {false};

        void m_ProcessInput();
        static void m_DisableAllKernelEffects();
        static void m_ProcessKeyPressedDown(Keys keyPressedDown);
    };
}
