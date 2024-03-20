#include "Window.h"

#include "Data.h"
#include "Error.h"

void Render::Window::Init() {
    m_glfwWindow = glfwCreateWindow(Data::WindowData::screenWidth, Data::WindowData::screenHeight,
                                    Data::WindowData::windowTitle, nullptr, nullptr);
    ASSERT(m_glfwWindow != nullptr, "Failed to create GLFW window");

    glfwMakeContextCurrent(m_glfwWindow);
    glfwSwapInterval(1);

    m_running = true;
}

bool Render::Window::IsRunning() const {
    return m_running;
}

void Render::Window::Update() {
    glfwSwapInterval(1);

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
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height); (void) window;
}
