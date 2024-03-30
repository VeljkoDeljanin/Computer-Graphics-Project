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
            switch (event.keyboard.key) {
                case GLFW_KEY_F:
                    if (event.keyboard.keyState == Controllers::KeyState::JustPressed) {
                        ProgramState::flashlight = !ProgramState::flashlight;
                    }
                    break;
                case GLFW_KEY_V:
                    if (event.keyboard.keyState == Controllers::KeyState::JustPressed) {
                        ProgramState::VSync = !ProgramState::VSync;
                    }
                    break;
            }
        }
    }

    m_eventQueue.clear();
}
