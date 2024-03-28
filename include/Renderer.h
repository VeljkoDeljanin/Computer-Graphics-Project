#pragma once

#include "Window.h"
#include "GuiWindow.h"
#include "Shader.h"
#include "Camera.h"
#include "Model/Model.h"

namespace Render {
    class Renderer {
    public:
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&) = delete;

        static Renderer& GetInstance() {
            static Renderer instance;
            return instance;
        }

        void Init();
        void Update();
        bool IsRunning();
    private:
        Renderer();

        float m_deltaTime = 0.0f;
        float m_lastFrame = 0.0f;

        Window &m_window;
        GuiWindow &m_guiWindow;
        Camera &m_camera;

        void m_UpdateWindows();

        static inline void m_CursorPosCallback(GLFWwindow *window, double xpos, double ypos);
        static inline void m_ScrollCallback(GLFWwindow *window, double xpos, double ypos);
        static inline void m_KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

        // For testing purposes
#if 1
        Shader shader;
        Model ourModel;
#endif
    };
}
