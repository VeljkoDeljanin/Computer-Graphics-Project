#include "Controllers/InputController.h"

#include <GLFW/glfw3.h>

#include "Controllers/EventController.h"
#include "Controllers/ServiceLocator.h"
#include "GuiWindow.h"

void Controllers::InputController::Update() {
    for (auto &key : m_keys) {
        auto &keyFrameState = key.second;
        int glfwKeyCode = key.first;
        int glfwKeyAction = keyFrameState.glfwKeyAction;
        bool keyStateChanged = false;
        switch (keyFrameState.keyState) {
            case KeyState::Released:
                if (glfwKeyAction == GLFW_PRESS) {
                    keyFrameState.keyState = KeyState::JustPressed;
                    keyStateChanged = true;
                }
                break;
            case KeyState::JustPressed:
                if (glfwKeyAction == GLFW_PRESS || glfwKeyAction == -1) {
                    keyFrameState.keyState = KeyState::Pressed;
                    keyStateChanged = true;
                }
                break;
            case KeyState::Pressed:
                if (glfwKeyAction == GLFW_RELEASE) {
                    keyFrameState.keyState = KeyState::JustReleased;
                    keyStateChanged = true;
                }
                break;
            case KeyState::JustReleased:
                if (glfwKeyAction == GLFW_RELEASE || glfwKeyAction == -1) {
                    keyFrameState.keyState = KeyState::Released;
                    keyStateChanged = true;
                }
                break;
        }
        if (keyStateChanged) {
            Event event{};
            event.eventType = EventType::Keyboard;
            event.keyboard.key = glfwKeyCode;
            event.keyboard.keyState = keyFrameState.keyState;

            Controllers::ServiceLocator::GetInstance().GetEventController().PushEvent(event);
        }
        keyFrameState.glfwKeyAction = -1;
    }
}

Controllers::KeyState Controllers::InputController::GetKeyState(int key) const {
    auto it = m_keys.find(key);
    return it != m_keys.end() ? it->second.keyState : KeyState::Released;
}

void Controllers::InputController::ProcessKeyCallback(GLFWwindow *window, int key, int action) {
    auto& keyState = m_keys[key];
    keyState.glfwKeyAction = action;
    (void) window;
}

void Controllers::InputController::ProcessMouseMovementCallback(double xpos, double ypos) {
    m_mouse.currentX = xpos;
    m_mouse.currentY = ypos;

    if (m_mouse.firstMouse) {
        m_mouse.lastX = m_mouse.currentX;
        m_mouse.lastY = m_mouse.currentY;
        m_mouse.firstMouse = false;
    }

    Event event{};
    event.eventType = EventType::MouseMoved;
    event.mouseMoved.xoffset = m_mouse.currentX - m_mouse.lastX;
    event.mouseMoved.yoffset = m_mouse.lastY - m_mouse.currentY;

    m_mouse.lastX = m_mouse.currentX;
    m_mouse.lastY = m_mouse.currentY;

    if (!Render::GuiWindow::IsCursorEnabled())
        Controllers::ServiceLocator::GetInstance().GetEventController().PushEvent(event);
}

void Controllers::InputController::ProcessMouseScrollCallback(double ypos) {
    m_scroll.lastY = m_scroll.currentY;
    m_scroll.currentY = ypos;

    Event event{};
    event.eventType = EventType::Scroll;
    event.scroll.yoffset = ypos;
//    event.scroll.yoffset = m_scroll.currentY - m_scroll.lastY;

    Controllers::ServiceLocator::GetInstance().GetEventController().PushEvent(event);
}

const char *Controllers::ToString(Controllers::KeyState state) {
    using KeyState = Controllers::KeyState;
    switch (state) {
        case KeyState::Released: return "Released";
        case KeyState::Pressed: return "Pressed";
        case KeyState::JustReleased: return "JustReleased";
        case KeyState::JustPressed: return "JustPressed";
        default: return "Input: default";
    }
}
