#include "GuiWindow.h"

#include <cmath>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Window.h"
#include "Data.h"
#include "Controllers/ServiceLocator.h"
#include "Camera.h"
#include "ProgramState.h"

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
    return ProgramState::cursorEnabled;
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
                        ProgramState::guiEnabled = !ProgramState::guiEnabled;
                        if (ProgramState::guiEnabled) {
                            ProgramState::cursorEnabled = true;
                            glfwSetInputMode(Window::GetGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                        }
                        else {
                            ProgramState::cursorEnabled = false;
                            glfwSetInputMode(Window::GetGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                        }
                    }
                    break;
                case GLFW_KEY_C:
                    if (event.keyboard.keyState == Controllers::KeyState::JustPressed && ProgramState::guiEnabled) {
                        ProgramState::cursorEnabled = !ProgramState::cursorEnabled;
                        if (!ProgramState::cursorEnabled)
                            glfwSetInputMode(Window::GetGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                        else
                            glfwSetInputMode(Window::GetGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                    }
                    break;
            }
        }
    }

    m_eventQueue.clear();
}

void Render::GuiWindow::m_Draw() {
    if (ProgramState::guiEnabled) {
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(ProgramState::guiWindowWidth, ProgramState::guiWindowHeight), ImGuiCond_Once);
        ImGui::Begin("Settings");

        if (ImGui::CollapsingHeader("General", ImGuiTreeNodeFlags_None)) {
            ImGui::Bullet(); ImGui::Checkbox("VSync (Shortcut: V)", &ProgramState::VSync);
            ImGui::Bullet(); ImGui::SliderFloat("Movement Speed", &ProgramState::movementSpeed, 1.0f, 10.0f);
            ImGui::SameLine(); m_HelpMarker("Use ARROW_LEFT/ARROW_RIGHT to decrease/increase movement speed.");
            Camera::GetInstance().SetMovementSpeed(ProgramState::movementSpeed);
            ImGui::Bullet(); ImGui::Checkbox("Mouse Cursor (Shortcut: C)", &ProgramState::cursorEnabled);
            if (!ProgramState::cursorEnabled)
                glfwSetInputMode(Window::GetGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            else
                glfwSetInputMode(Window::GetGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_None)) {
            Camera &camera = Camera::GetInstance();
            ImGui::Text("Camera Info:");
            ImGui::Indent();
            ImGui::Bullet(); ImGui::Text("Camera position: (%f, %f, %f)", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
            ImGui::Bullet(); ImGui::Text("(Yaw, Pitch): (%f, %f)", camera.GetYaw(), camera.GetPitch());
            ImGui::Bullet(); ImGui::Text("Camera front: (%f, %f, %f)", camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
            ImGui::Bullet(); ImGui::Text("Zoom: %f", camera.GetZoom());
            ImGui::Unindent();
            ImGui::Bullet(); ImGui::Checkbox("Fly Camera (Shortcut: N)", &ProgramState::flyCamera);
        }

        if (ImGui::CollapsingHeader("Lights", ImGuiTreeNodeFlags_None)) {
            ImGui::Bullet(); ImGui::Checkbox("Flashlight (Shortcut: F)", &ProgramState::flashlight);
            ImGui::Bullet(); ImGui::Checkbox("Lamp lights (Shortcut: L)", &ProgramState::lampLights);
            ImGui::Bullet(); ImGui::Checkbox("Billboard lights (Shortcut: K)", &ProgramState::billboardLights);
        }

        if (ImGui::CollapsingHeader("Post-Processing", ImGuiTreeNodeFlags_None)) {
            ImGui::Bullet(); ImGui::Checkbox("Anti-Aliasing (Shortcut: F2)", &ProgramState::antiAliasing);
            if (ProgramState::antiAliasing)
                glEnable(GL_MULTISAMPLE);
            else
                glDisable(GL_MULTISAMPLE);
            ImGui::Bullet(); ImGui::Checkbox("Grayscale (Shortcut: F3)", &ProgramState::grayscale);
            m_kernelBefore = ProgramState::kernelSelected;
            if (ProgramState::sharpenKernel) ProgramState::kernelSelected = 1;
            else if (ProgramState::blurKernel) ProgramState::kernelSelected = 2;
            else if (ProgramState::edgeDetectionKernel) ProgramState::kernelSelected = 3;
            else if (ProgramState::embossKernel) ProgramState::kernelSelected = 4;
            else ProgramState::kernelSelected = 0;
            ImGui::Text("Kernel effects:"); ImGui::SameLine();
            m_HelpMarker("Only one kernel effect can be active at a time!");
            ImGui::Indent();
            ImGui::Bullet(); ImGui::RadioButton("Identity (Shortcut: F4)", &ProgramState::kernelSelected, 0);
            ImGui::Bullet(); ImGui::RadioButton("Sharpen (Shortcut: F5)", &ProgramState::kernelSelected, 1);
            ImGui::Bullet(); ImGui::RadioButton("Blur (Shortcut: F6)", &ProgramState::kernelSelected, 2);
            ImGui::Bullet(); ImGui::RadioButton("Edge Detection (Shortcut: F7)", &ProgramState::kernelSelected, 3);
            ImGui::Bullet(); ImGui::RadioButton("Emboss (Shortcut: F8)", &ProgramState::kernelSelected, 4);
            ImGui::Unindent();
            if (m_kernelBefore != ProgramState::kernelSelected) {
                ProgramState::sharpenKernel = false;
                ProgramState::blurKernel = false;
                ProgramState::edgeDetectionKernel = false;
                ProgramState::embossKernel = false;
                switch (ProgramState::kernelSelected) {
                    case 1:
                        ProgramState::sharpenKernel = true;
                        break;
                    case 2:
                        ProgramState::blurKernel = true;
                        break;
                    case 3:
                        ProgramState::edgeDetectionKernel = true;
                        break;
                    case 4:
                        ProgramState::embossKernel = true;
                        break;
                    default:
                        break;
                }
            }
            ImGui::Bullet(); ImGui::Checkbox("HDR (Shortcut: H)", &ProgramState::hdr);
            ImGui::Bullet(); ImGui::Checkbox("Bloom (Shortcut: B)", &ProgramState::bloom);
            ImGui::Bullet(); ImGui::SliderFloat("Exposure", &ProgramState::exposure, 0.1f, 10.0f);
            ImGui::SameLine(); m_HelpMarker("Use Q/E to decrease/increase exposure level.");
        }

        if (ImGui::CollapsingHeader("Normal and Parallax Mapping", ImGuiTreeNodeFlags_None)) {
            ImGui::Bullet(); ImGui::Checkbox("Normal Mapping (Shortcut: O)", &ProgramState::normalMapping);
            if (ProgramState::normalMapping) {
                ImGui::Bullet(); ImGui::Checkbox("Parallax Mapping (Shortcut: P)", &ProgramState::parallaxMapping);
            }
            if (!ProgramState::normalMapping)
                ProgramState::parallaxMapping = false;
            ImGui::Bullet(); ImGui::SliderFloat("Height Scale", &ProgramState::heightScale, 0.05f, 0.3f);
            ImGui::SameLine(); m_HelpMarker("Use ARROW_DOWN/ARROW_UP to decrease/increase height scale.");
        }

        ImGui::End();
    }

    { // Settings shortcut
        ImGui::SetNextWindowBgAlpha(0.35f);
        ImGui::SetNextWindowPos(ImVec2(static_cast<float>(Data::WindowData::screenWidth/2.0 - 46), 0));
        ImGui::SetNextWindowSize(ImVec2(94, 50));
        ImGui::Begin("Settings: F1", nullptr,
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground);
        ImGui::End();
    }

    { // FPS counter
        ImGui::SetNextWindowBgAlpha(0.35f);
        ImGui::SetNextWindowPos(ImVec2(static_cast<float>(Data::WindowData::screenWidth - 60), 0));
        ImGui::SetNextWindowSize(ImVec2(60, 50));
        ImGui::Begin("FPS:", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
        ImGui::Text("%.2f", std::floor(1 / m_deltaTime));
        ImGui::End();
    }
}

void Render::GuiWindow::m_HelpMarker(const char *desc) {
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}
