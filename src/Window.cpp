#include "Window.h"

#include "Data.h"
#include "Error.h"
#include "Controllers/ServiceLocator.h"
#include "ProgramState.h"

void Render::Window::Notify(Controllers::Event event) {
    m_eventQueue.push_back(event);
}

void Render::Window::Init() {
    m_glfwWindow = glfwCreateWindow(Data::WindowData::screenWidth, Data::WindowData::screenHeight,
                                    Data::WindowData::windowTitle, nullptr, nullptr);
    ASSERT(m_glfwWindow != nullptr, "Failed to create GLFW window");

    glfwMakeContextCurrent(m_glfwWindow);
    glfwSwapInterval(ProgramState::VSync ? 1 : 0);

    m_running = true;
}

bool Render::Window::IsRunning() const {
    return m_running;
}

void Render::Window::Update() {
    m_ProcessInput();
    glfwSwapInterval(ProgramState::VSync ? 1 : 0);

    if (!glfwWindowShouldClose(m_glfwWindow)) {
        glfwSwapBuffers(m_glfwWindow);
        return;
    }

    m_running = false;
}

void Render::Window::Destroy() {
    glfwDestroyWindow(m_glfwWindow);
    m_running = false;
}

GLFWwindow *Render::Window::GetGlfwWindow() {
    return m_glfwWindow;
}

void Render::Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); (void) window;
}

Render::Window::Window() {
    Controllers::ServiceLocator::GetInstance().GetEventController().SubscribeToEvent(
            Controllers::EventType::Keyboard, this
    );
}

void Render::Window::m_ProcessInput() {
    for (auto& event : m_eventQueue) {
        if (event.eventType == Controllers::EventType::Keyboard) {
            if (event.keyboard.keyState == Controllers::KeyState::JustPressed) {
                switch (event.keyboard.key) {
                    case GLFW_KEY_F:
                        ProgramState::flashlight = !ProgramState::flashlight;
                        break;
                    case GLFW_KEY_V:
                        ProgramState::VSync = !ProgramState::VSync;
                        break;
                    case GLFW_KEY_F3:
                        if (!ProgramState::inversion)
                            m_DisableAllPostProcessingEffects();
                        ProgramState::inversion = !ProgramState::inversion;
                        break;
                    case GLFW_KEY_F4:
                        if (!ProgramState::grayscale)
                            m_DisableAllPostProcessingEffects();
                        ProgramState::grayscale = !ProgramState::grayscale;
                        break;
                    case GLFW_KEY_F5:
                        if (!ProgramState::sharpenKernel)
                            m_DisableAllPostProcessingEffects();
                        ProgramState::sharpenKernel = !ProgramState::sharpenKernel;
                        break;
                    case GLFW_KEY_F6:
                        if (!ProgramState::blurKernel)
                            m_DisableAllPostProcessingEffects();
                        ProgramState::blurKernel = !ProgramState::blurKernel;
                        break;
                    case GLFW_KEY_F7:
                        if (!ProgramState::edgeDetectionKernel)
                            m_DisableAllPostProcessingEffects();
                        ProgramState::edgeDetectionKernel = !ProgramState::edgeDetectionKernel;
                        break;
                }
            }
        }
    }

    m_eventQueue.clear();
}

void Render::Window::m_DisableAllPostProcessingEffects() {
    ProgramState::inversion = false;
    ProgramState::grayscale = false;
    ProgramState::sharpenKernel = false;
    ProgramState::blurKernel = false;
    ProgramState::edgeDetectionKernel = false;
}
