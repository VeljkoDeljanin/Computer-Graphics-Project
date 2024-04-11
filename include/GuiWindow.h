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

        static void Init();
        void Update(float deltaTime);
        static void Destroy();

        static bool IsCursorEnabled();
    private:
        GuiWindow();

        float m_deltaTime{};
        std::vector<Controllers::Event> m_eventQueue;
        int m_kernelBefore{};

        void m_ProcessInput();
        void m_Draw();

        static void m_HelpMarker(const char *desc);
    };
}
