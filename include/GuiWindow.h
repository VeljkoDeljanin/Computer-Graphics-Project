#pragma once

namespace Render {
    class GuiWindow {
    public:
        static GuiWindow& GetInstance() {
            static GuiWindow instance;
            return instance;
        }

        void Init();
        void Update(float deltaTime);
        void Destroy();

        GuiWindow(const GuiWindow&) = delete;
        GuiWindow(GuiWindow&&) = delete;
        GuiWindow& operator=(const GuiWindow&) = delete;
        GuiWindow& operator=(GuiWindow&&) = delete;
    private:
        float m_deltaTime;

        GuiWindow() = default;

        void m_DrawMenu();
    };
}
