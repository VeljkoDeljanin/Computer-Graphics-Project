#include "GuiWindow.h"

#include <cmath>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Window.h"
#include "Data.h"

void Render::GuiWindow::Init() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void) io;
    ImGui_ImplGlfw_InitForOpenGL(Window::GetGlfwWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460 core");
}

void Render::GuiWindow::Update(float deltaTime) {
    m_deltaTime = deltaTime;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    m_DrawMenu();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Render::GuiWindow::Destroy() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Render::GuiWindow::m_DrawMenu() {
    {
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(300, 150));
        ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
        ImGui::Begin("Hello window");
        ImGui::Text("Hello text");
        ImGui::End();
    }

    { // FPS
        ImGui::SetNextWindowBgAlpha(0.35f);
        ImGui::SetNextWindowPos(ImVec2(static_cast<float>(Data::WindowData::screenWidth - 70), 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(70, 50));
        ImGui::SetNextWindowCollapsed(false, ImGuiCond_Always);
        ImGui::Begin("FPS");
        ImGui::Text("%.2f", std::floor(1 / m_deltaTime));
        ImGui::End();
    }
}
