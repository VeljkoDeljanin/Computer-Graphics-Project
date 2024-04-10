#include "Entities/LightBox.h"

#include <glad/glad.h>

#include "Data.h"
#include "Camera.h"
#include "Model/Model.h"

Entities::LightBox::LightBox(std::shared_ptr<Render::Shader> shader) : m_shader(std::move(shader)) {
    m_textures.push_back(Render::TextureFromFile("billboard1.jpg", "resources/textures/billboard"));
    m_textures.push_back(Render::TextureFromFile("billboard2.jpg", "resources/textures/billboard"));
    m_textures.push_back(Render::TextureFromFile("billboard3.jpg", "resources/textures/billboard"));
    m_textures.push_back(Render::TextureFromFile("billboard4.jpg", "resources/textures/billboard"));

    m_shader->ActivateShader();
    m_shader->SetInt("image", 0);
    m_shader->DeactivateShader();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Data::CubeData::vertices), Data::CubeData::vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Entities::LightBox::~LightBox() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Entities::LightBox::Update() {
    m_shader->ActivateShader();

    view = Render::Camera::GetInstance().GetViewMatrix();
    m_shader->SetMat4("view", view);

    projection = glm::mat4(glm::perspective(glm::radians(Render::Camera::GetInstance().GetZoom()),
                                            static_cast<float>(Data::WindowData::screenWidth) / static_cast<float>(Data::WindowData::screenHeight),
                                            0.1f, 100.0f));
    m_shader->SetMat4("projection", projection);

    m_lightColor = glm::vec3(15.0f);
    m_shader->SetVec3("lightColor", m_lightColor);

    glBindVertexArray(VAO);

    for (int i = 0; i < 4; i++) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, m_lightBoxPositions[i]);
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        if (i == 0)
            model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        else if (i == 1)
            model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        else if (i == 2)
            model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        else if (i == 3)
            model = glm::rotate(model, glm::radians(-135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, m_lightBoxScales[i]);
        m_shader->SetMat4("model", model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textures[i]);

        glDrawArrays(GL_TRIANGLES, 0 ,36);
    }

    glBindVertexArray(0);

    m_shader->DeactivateShader();
}
