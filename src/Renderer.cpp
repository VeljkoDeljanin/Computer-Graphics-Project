#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Error.h"

void Render::Renderer::Init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_window.Init();

    glfwSetFramebufferSizeCallback(Render::Window::GetGlfwWindow(), Render::Window::FramebufferSizeCallback);

    ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize GLAD");

    m_guiWindow.Init();

    m_deltaTime = 0.0f;
    m_lastFrame = 0.0f;
}

void Render::Renderer::Update() {
    auto currentFrame = static_cast<float>(glfwGetTime());
    m_deltaTime = currentFrame - m_lastFrame;
    m_lastFrame = currentFrame;

    if(glfwGetKey(Render::Window::GetGlfwWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(Render::Window::GetGlfwWindow(), true);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_UpdateWindows();
    glfwPollEvents();
}

bool Render::Renderer::IsRunning() {
    return m_window.IsRunning();
}

Render::Renderer::~Renderer() {
    m_window.Destroy();
    m_guiWindow.Destroy();
    glfwTerminate();
}

void Render::Renderer::m_UpdateWindows() {
    m_guiWindow.Update(m_deltaTime);
    m_window.Update();
}

Render::Renderer::Renderer()
: m_window(Render::Window::GetInstance()), m_guiWindow(Render::GuiWindow::GetInstance()) {

}
