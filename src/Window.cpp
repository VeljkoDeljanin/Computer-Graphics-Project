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
    Data::WindowData::screenWidth = width;
    Data::WindowData::screenHeight = height;
    (void) window;
}

Render::Window::Window() {
    Controllers::ServiceLocator::GetInstance().GetEventController().SubscribeToEvent(
            Controllers::EventType::Keyboard, this
    );
}

void Render::Window::m_ProcessInput() {
    for (auto& event : m_eventQueue) {
        if (event.eventType == Controllers::EventType::Keyboard) {
            if (event.keyboard.key == GLFW_KEY_Q) {
                m_keysPressedDown[static_cast<int>(Render::Keys::KEY_Q)]
                    = event.keyboard.keyState == Controllers::KeyState::Pressed;
            }
            else if (event.keyboard.key == GLFW_KEY_E) {
                m_keysPressedDown[static_cast<int>(Render::Keys::KEY_E)]
                        = event.keyboard.keyState == Controllers::KeyState::Pressed;
            }
            else if (event.keyboard.key == GLFW_KEY_UP) {
                m_keysPressedDown[static_cast<int>(Render::Keys::KEY_ARROW_UP)]
                        = event.keyboard.keyState == Controllers::KeyState::Pressed;
            }
            else if (event.keyboard.key == GLFW_KEY_DOWN) {
                m_keysPressedDown[static_cast<int>(Render::Keys::KEY_ARROW_DOWN)]
                        = event.keyboard.keyState == Controllers::KeyState::Pressed;
            }
            else if (event.keyboard.keyState == Controllers::KeyState::JustPressed) {
                switch (event.keyboard.key) {
                    case GLFW_KEY_F:
                        ProgramState::flashlight = !ProgramState::flashlight;
                        break;
                    case GLFW_KEY_V:
                        ProgramState::VSync = !ProgramState::VSync;
                        break;
                    case GLFW_KEY_F3:
                        ProgramState::antiAliasing = !ProgramState::antiAliasing;
                        if (ProgramState::antiAliasing)
                            glEnable(GL_MULTISAMPLE);
                        else
                            glDisable(GL_MULTISAMPLE);
                        break;
                    case GLFW_KEY_F4:
                        ProgramState::grayscale = !ProgramState::grayscale;
                        break;
                    case GLFW_KEY_F5:
                        if (!ProgramState::sharpenKernel)
                            m_DisableAllKernelEffects();
                        ProgramState::sharpenKernel = !ProgramState::sharpenKernel;
                        break;
                    case GLFW_KEY_F6:
                        if (!ProgramState::blurKernel)
                            m_DisableAllKernelEffects();
                        ProgramState::blurKernel = !ProgramState::blurKernel;
                        break;
                    case GLFW_KEY_F7:
                        if (!ProgramState::edgeDetectionKernel)
                            m_DisableAllKernelEffects();
                        ProgramState::edgeDetectionKernel = !ProgramState::edgeDetectionKernel;
                        break;
                    case GLFW_KEY_F8:
                        if (!ProgramState::embossKernel)
                            m_DisableAllKernelEffects();
                        ProgramState::embossKernel = !ProgramState::embossKernel;
                        break;
                    case GLFW_KEY_O:
                        if (ProgramState::normalMapping)
                            ProgramState::parallaxMapping = false;
                        ProgramState::normalMapping = !ProgramState::normalMapping;
                        break;
                    case GLFW_KEY_P:
                        if (ProgramState::normalMapping)
                            ProgramState::parallaxMapping = !ProgramState::parallaxMapping;
                        break;
                    case GLFW_KEY_H:
                        ProgramState::hdr = !ProgramState::hdr;
                        break;
                    case GLFW_KEY_B:
                        ProgramState::bloom = !ProgramState::bloom;
                        break;
                }
            }
        }
    }

    for (int key = 0; key < 4; key++)
        if (m_keysPressedDown[key])
            m_ProcessKeyPressedDown(static_cast<Keys>(key));

    m_eventQueue.clear();
}

void Render::Window::m_DisableAllKernelEffects() {
    ProgramState::sharpenKernel = false;
    ProgramState::blurKernel = false;
    ProgramState::edgeDetectionKernel = false;
    ProgramState::embossKernel = false;
}

void Render::Window::m_ProcessKeyPressedDown(Keys keyPressedDown) {
    switch (keyPressedDown) {
        case Keys::KEY_Q:
            if (ProgramState::exposure > 0.1f)
                ProgramState::exposure -= 0.005f;
            else
                ProgramState::exposure = 0.1f;
            break;
        case Keys::KEY_E:
            if (ProgramState::exposure < 10.0f)
                ProgramState::exposure += 0.005f;
            else
                ProgramState::exposure = 10.0f;
            break;
        case Keys::KEY_ARROW_UP:
            if (ProgramState::heightScale < 0.3f)
                ProgramState::heightScale += 0.001f;
            else
                ProgramState::heightScale = 0.3f;
            break;
        case Keys::KEY_ARROW_DOWN:
            if (ProgramState::heightScale > 0.05f)
                ProgramState::heightScale -= 0.001f;
            else
                ProgramState::heightScale = 0.05f;
            break;
    }
}
