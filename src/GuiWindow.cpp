#include "GuiWindow.h"

#include <cmath>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Window.h"
#include "Data.h"
#include "Controllers/ServiceLocator.h"
#include "Camera.h"

void Render::GuiWindow::Notify(Controllers::Event event) {
    m_eventQueue.push_back(event);
}

void Render::GuiWindow::Init() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void) io;
    ImGui_ImplGlfw_InitForOpenGL(Window::GetGlfwWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460 core");
}

void Render::GuiWindow::Update(float deltaTime) {
    m_ProcessInput();
    m_deltaTime = deltaTime;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    m_Draw();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Render::GuiWindow::Destroy() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

bool Render::GuiWindow::IsCursorEnabled() {
    return m_cursorEnabled;
}

Render::GuiWindow::GuiWindow() {
    Controllers::ServiceLocator::GetInstance().GetEventController().SubscribeToEvent(
            Controllers::EventType::Keyboard, this
    );
}

void Render::GuiWindow::m_ProcessInput() {
    for (auto& event : m_eventQueue) {
        if (event.eventType == Controllers::EventType::Keyboard) {
            switch (event.keyboard.key) {
                case GLFW_KEY_F1:
                    if (event.keyboard.keyState == Controllers::KeyState::JustPressed) {
                        m_guiEnabled = !m_guiEnabled;
                        if (m_guiEnabled) {
                            m_cursorEnabled = true;
                            glfwSetInputMode(Render::Window::GetGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                        }
                        else {
                            m_cursorEnabled = false;
                            glfwSetInputMode(Render::Window::GetGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                        }
                    }
                    break;
                case GLFW_KEY_C:
                    if (event.keyboard.keyState == Controllers::KeyState::JustPressed && m_guiEnabled) {
                        m_cursorEnabled = !m_cursorEnabled;
                        if (!m_cursorEnabled)
                            glfwSetInputMode(Render::Window::GetGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                        else
                            glfwSetInputMode(Render::Window::GetGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                    }
                    break;
            }
        }
    }

    m_eventQueue.clear();
}

void Render::GuiWindow::m_Draw() const {
    if (m_guiEnabled) {
        {
            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(400, 150));
            ImGui::SetNextWindowCollapsed(false, ImGuiCond_Once);
            ImGui::Begin("Camera info");
            Camera &camera = Camera::GetInstance();
            ImGui::Text("Camera position: (%f, %f, %f)", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
            ImGui::Text("(Yaw, Pitch): (%f, %f)", camera.GetYaw(), camera.GetPitch());
            ImGui::Text("Camera front: (%f, %f, %f)", camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
            ImGui::End();
        }
    }

    { // FPS counter
        ImGui::SetNextWindowBgAlpha(0.35f);
        ImGui::SetNextWindowPos(ImVec2(static_cast<float>(Data::WindowData::screenWidth - 70), 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(70, 50));
        ImGui::SetNextWindowCollapsed(false, ImGuiCond_Always);
        ImGui::Begin("FPS");
        ImGui::Text("%.2f", std::floor(1 / m_deltaTime));
        ImGui::End();
    }
}
