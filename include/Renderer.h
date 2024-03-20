#pragma once

#include "Window.h"
#include "GuiWindow.h"

namespace Render {
    class Renderer {
    public:
        static Renderer& GetInstance() {
            static Renderer instance;
            return instance;
        }

        void Init();
        void Update();
        bool IsRunning();

        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&) = delete;
    private:
        float m_deltaTime;
        float m_lastFrame;

        Render::Window &m_window;
        Render::GuiWindow &m_guiWindow;

        void m_UpdateWindows();

        Renderer();
    };
}
