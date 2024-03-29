#include "Entities/Skybox.h"

#include <utility>

#include <stb_image.h>

#include "Error.h"
#include "Data.h"
#include "Camera.h"

Entities::Skybox::Skybox(std::shared_ptr<Render::Shader> shader)
: projection(glm::perspective(glm::radians(Render::Camera::GetInstance().GetZoom()),
                              static_cast<float>(Data::WindowData::screenWidth) / static_cast<float>(Data::WindowData::screenHeight),
                              0.1f, 100.0f)) {
    m_shader = std::move(shader);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Data::SkyboxData::vertices), &Data::SkyboxData::vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)nullptr);

    m_textureID = m_LoadCubemap();

    m_shader->ActivateShader();
    m_shader->SetInt("skybox", 0);
    m_shader->SetMat4("projection", projection);
    m_shader->DeactivateShader();
}

Entities::Skybox::~Skybox() {
    glDeleteTextures(1, &m_textureID);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Entities::Skybox::Update() {
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);

    m_shader->ActivateShader();

    view = glm::mat4(glm::mat3(Render::Camera::GetInstance().GetViewMatrix()));
    m_shader->SetMat4("view", view);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    m_shader->DeactivateShader();

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}

unsigned int Entities::Skybox::m_LoadCubemap() {
    stbi_set_flip_vertically_on_load(true);

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrComponents;
    for (unsigned int i = 0; i < Data::SkyboxData::faces.size(); i++) {
        unsigned char *data = stbi_load(Data::SkyboxData::faces[i].c_str(), &width, &height, &nrComponents, 0);

        if (data)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        else
            ASSERT(false, "Cubemap texture failed to load at path: " + Data::SkyboxData::faces[i]);

        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
