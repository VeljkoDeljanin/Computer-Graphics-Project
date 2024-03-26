#pragma once

#include "Controllers/EventController.h"

namespace Render {
    class GuiWindow : public Controllers::Observer {
    public:
        GuiWindow(const GuiWindow&) = delete;
        GuiWindow(GuiWindow&&) = delete;
        GuiWindow& operator=(const GuiWindow&) = delete;
        GuiWindow& operator=(GuiWindow&&) = delete;

        static GuiWindow& GetInstance() {
            static GuiWindow instance;
            return instance;
        }

        void Notify(Controllers::Event event) override;

        void Init();
        void Update(float deltaTime);
        void Destroy();

        static bool IsCursorEnabled();
    private:
        GuiWindow();

        float m_deltaTime = {};
        volatile bool m_guiEnabled = false;
        static inline bool m_cursorEnabled = false;
        std::vector<Controllers::Event> m_eventQueue;

        void m_ProcessInput();
        void m_Draw() const;
    };
}
