#include "Framebuffer.h"

#include <glad/glad.h>

#include "Data.h"
#include "Error.h"
#include "ProgramState.h"

void Render::Framebuffer::Init() {
    m_shader = new Shader("resources/shaders/postProcessing.vs", "resources/shaders/postProcessing.fs");

    m_shader->ActivateShader();
    m_shader->SetInt("screenTexture", 0);
    Shader::DeactivateShader();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Data::FramebufferData::vertices),
                 &Data::FramebufferData::vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));


    glGenFramebuffers(1, &m_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

    glGenTextures(1, &m_textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_textureColorBuffer);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, Data::FramebufferData::framebufferWidth,
                            Data::FramebufferData::framebufferHeight, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_textureColorBuffer, 0);

    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8,
                                     Data::FramebufferData::framebufferWidth, Data::FramebufferData::framebufferHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);


    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
           "ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Render::Framebuffer::Clear() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteRenderbuffers(1, &RBO);
    glDeleteFramebuffers(1, &m_framebuffer);
    delete m_shader;
}

void Render::Framebuffer::Bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    glEnable(GL_DEPTH_TEST);
}

void Render::Framebuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_shader->ActivateShader();

    m_shader->SetInt("framebufferWidth", Data::FramebufferData::framebufferWidth);
    m_shader->SetInt("framebufferHeight", Data::FramebufferData::framebufferHeight);

    m_shader->SetBool("grayscaleEnabled", ProgramState::grayscale);

    m_shader->SetBool("sharpenKernelEnabled", ProgramState::sharpenKernel);
    m_shader->SetBool("blurKernelEnabled", ProgramState::blurKernel);
    m_shader->SetBool("edgeDetectionKernelEnabled", ProgramState::edgeDetectionKernel);
    m_shader->SetBool("embossKernelEnabled", ProgramState::embossKernel);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_textureColorBuffer);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    Shader::DeactivateShader();
}
