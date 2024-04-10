#include "Framebuffer.h"

#include <glad/glad.h>

#include "Data.h"
#include "Error.h"
#include "ProgramState.h"

void Render::Framebuffer::Init() {
    m_postProcessingShader = new Shader("resources/shaders/postProcessing.vs", "resources/shaders/postProcessing.fs");
    m_blurShader = new Shader("resources/shaders/blur.vs", "resources/shaders/blur.fs");

    m_postProcessingShader->ActivateShader();
    m_postProcessingShader->SetInt("hdrBuffer", 0);
    m_postProcessingShader->SetInt("bloomBlur", 1);
    m_postProcessingShader->SetInt("screenTexture", 2);
    Shader::DeactivateShader();

    m_blurShader->ActivateShader();
    m_blurShader->SetInt("image", 0);
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


    glGenTextures(2, m_colorBuffers);
    for (int i = 0; i < 2; i++) {
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_colorBuffers[i]);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA16F, Data::FramebufferData::framebufferWidth,
                                Data::FramebufferData::framebufferHeight, GL_TRUE);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
                               GL_TEXTURE_2D_MULTISAMPLE, m_colorBuffers[i], 0);
    }

    glGenTextures(1, &m_textureColorBufferMultiSampled);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_textureColorBufferMultiSampled);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, Data::FramebufferData::framebufferWidth,
                            Data::FramebufferData::framebufferHeight, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D_MULTISAMPLE, m_textureColorBufferMultiSampled, 0);


    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8,
                                     Data::FramebufferData::framebufferWidth, Data::FramebufferData::framebufferHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
           "ERROR::FRAMEBUFFER Framebuffer is not complete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenFramebuffers(2, m_pingpongFBO);
    glGenTextures(2, m_pingpongColorBuffers);
    for (int i = 0; i < 2; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_pingpongColorBuffers[i]);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA16F, Data::FramebufferData::framebufferWidth,
                                Data::FramebufferData::framebufferHeight, GL_TRUE);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D_MULTISAMPLE, m_pingpongColorBuffers[i], 0);

        ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
               "ERROR::FRAMEBUFFER Pingpong framebuffer is not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Render::Framebuffer::Clear() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteRenderbuffers(1, &RBO);

    glDeleteFramebuffers(1, &m_framebuffer);
    glDeleteFramebuffers(2, m_pingpongFBO);

    glDeleteTextures(1, &m_textureColorBufferMultiSampled);
    glDeleteTextures(2, m_colorBuffers);
    glDeleteTextures(2, m_pingpongColorBuffers);

    delete m_postProcessingShader;
}

void Render::Framebuffer::Bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    glEnable(GL_DEPTH_TEST);
}

void Render::Framebuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    horizontal = true;
    firstIteration = true;
    amount = 10;

    m_blurShader->ActivateShader();

    m_blurShader->SetInt("framebufferWidth", Data::FramebufferData::framebufferWidth);
    m_blurShader->SetInt("framebufferHeight", Data::FramebufferData::framebufferHeight);

    glBindVertexArray(VAO);
    for (unsigned int i = 0; i < amount; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_pingpongFBO[horizontal]);
        m_blurShader->SetInt("horizontal", horizontal);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, firstIteration ? m_colorBuffers[1] : m_pingpongColorBuffers[!horizontal]);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        horizontal = !horizontal;
        if (firstIteration)
            firstIteration = false;
    }

    Shader::DeactivateShader();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_postProcessingShader->ActivateShader();

    m_postProcessingShader->SetInt("framebufferWidth", Data::FramebufferData::framebufferWidth);
    m_postProcessingShader->SetInt("framebufferHeight", Data::FramebufferData::framebufferHeight);

    m_postProcessingShader->SetBool("grayscaleEnabled", ProgramState::grayscale);

    m_postProcessingShader->SetBool("sharpenKernelEnabled", ProgramState::sharpenKernel);
    m_postProcessingShader->SetBool("blurKernelEnabled", ProgramState::blurKernel);
    m_postProcessingShader->SetBool("edgeDetectionKernelEnabled", ProgramState::edgeDetectionKernel);
    m_postProcessingShader->SetBool("embossKernelEnabled", ProgramState::embossKernel);

    m_postProcessingShader->SetBool("hdr", ProgramState::hdr);
    m_postProcessingShader->SetBool("bloom", ProgramState::bloom);
    m_postProcessingShader->SetFloat("exposure", ProgramState::exposure);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_colorBuffers[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_pingpongColorBuffers[!horizontal]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_textureColorBufferMultiSampled);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    Shader::DeactivateShader();
}
