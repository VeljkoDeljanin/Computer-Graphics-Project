#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Error.h"
#include "FileSystem.h"
#include "Controllers/ServiceLocator.h"

Render::Renderer::~Renderer() {
    m_window.Destroy();
    m_guiWindow.Destroy();
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

    m_guiWindow.Init();

    m_deltaTime = 0.0f;
    m_lastFrame = 0.0f;

    // For testing purposes
#if 1
    shader.Compile("resources/shaders/shader.vs", "resources/shaders/shader.fs");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    LoadAndCreateTexture();
    shader.ActivateShader();
    shader.SetInt("texture1", 0);
    shader.SetInt("texture2", 1);
#endif
}

void Render::Renderer::Update() {
    auto currentFrame = static_cast<float>(glfwGetTime());
    m_deltaTime = currentFrame - m_lastFrame;
    m_lastFrame = currentFrame;

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_camera.Update(m_deltaTime);

#if 1

    shader.ActivateShader();

    glm::mat4 projection = glm::perspective(glm::radians(m_camera.GetZoom()), (float)1600 / (float)900, 0.1f, 100.0f);
    shader.SetMat4("projection", projection);

    glm::mat4 view = m_camera.GetViewMatrix();
    shader.SetMat4("view", view);

//    glm::mat4 model = glm::mat4(1.0f);
//    shader.SetMat4("model", model);

    // render box
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glBindVertexArray(VAO);
    for (unsigned int i = 0; i < 10; i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * (float)i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        shader.SetMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }


    shader.DeactivateShader();
#endif

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

// For testing purposes
#if 1
    void Render::Renderer::LoadAndCreateTexture() {
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(FileSystem::GetPath("resources/textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load(FileSystem::GetPath("resources/textures/awesomeface.png").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

#endif
