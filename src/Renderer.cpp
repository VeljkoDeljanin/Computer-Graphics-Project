#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Error.h"
#include "Controllers/ServiceLocator.h"

Render::Renderer::~Renderer() {
    Controllers::ServiceLocator::GetInstance().GetEntityController().Clear();
    m_window.Destroy();
    Render::GuiWindow::Destroy();
    glfwTerminate();
}

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
    glfwSetCursorPosCallback(Render::Window::GetGlfwWindow(), m_CursorPosCallback);
    glfwSetScrollCallback(Render::Window::GetGlfwWindow(), m_ScrollCallback);
    glfwSetKeyCallback(Render::Window::GetGlfwWindow(), m_KeyCallback);

    glfwSetInputMode(Render::Window::GetGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize GLAD");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    Render::GuiWindow::Init();

    Controllers::ServiceLocator::GetInstance().GetEntityController().Init();
}

void Render::Renderer::Update() {
    auto currentFrame = static_cast<float>(glfwGetTime());
    m_deltaTime = currentFrame - m_lastFrame;
    m_lastFrame = currentFrame;

    m_camera.Update(m_deltaTime);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Controllers::ServiceLocator::GetInstance().GetEntityController().Update();

    m_UpdateWindows();
    glfwPollEvents();
    Controllers::ServiceLocator::GetInstance().GetInputController().Update();
}

bool Render::Renderer::IsRunning() {
    return m_window.IsRunning();
}

Render::Renderer::Renderer()
: m_window(Render::Window::GetInstance()), m_guiWindow(Render::GuiWindow::GetInstance()),
  m_camera(Render::Camera::GetInstance()) {

}

void Render::Renderer::m_UpdateWindows() {
    m_guiWindow.Update(m_deltaTime);
    m_window.Update();
}

void Render::Renderer::m_CursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
    Controllers::ServiceLocator::GetInstance().GetInputController().ProcessMouseMovementCallback(xpos, ypos);
    (void) window;
}

void Render::Renderer::m_ScrollCallback(GLFWwindow *window, double xpos, double ypos) {
    Controllers::ServiceLocator::GetInstance().GetInputController().ProcessMouseScrollCallback(ypos);
    (void) window; (void) xpos;
}

void Render::Renderer::m_KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    Controllers::ServiceLocator::GetInstance().GetInputController().ProcessKeyCallback(window, key, action);
    (void) scancode; (void) mods;
}
