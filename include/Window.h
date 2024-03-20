#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Render {
    class Window {
    public:
        static Window& GetInstance() {
            static Window instance;
            return instance;
        }

        void Init();
        [[nodiscard]] bool IsRunning() const;
        void Update();
        void Destroy();

        static GLFWwindow *GetGlfwWindow();

        static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

        Window(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&) = delete;
    private:
        static inline GLFWwindow *m_glfwWindow = nullptr;
        bool m_running = false;

        Window() = default;
    };
}
