#include "Entities/Ground.h"

#include <utility>

#include <glad/glad.h>

#include "Model/Model.h"
#include "Camera.h"
#include "ProgramState.h"

Entities::Ground::Ground(std::shared_ptr<Render::Shader> shader, std::shared_ptr<Render::Shader> shader2,
                         std::shared_ptr<Render::Shader> shader3)
: m_shader(std::move(shader)), m_normalMapShader(std::move(shader2)), m_normalAndHeightMapShader(std::move(shader3)) {
    m_SetupGrassGround();
    m_SetupCobblestoneGround();

    m_grassDiffMap = Render::TextureFromFile("grass_path_diff.png", "resources/textures/grass_ground");
    m_grassSpecMap = Render::TextureFromFile("grass_path_spec.png", "resources/textures/grass_ground");
    m_grassNormMap = Render::TextureFromFile("grass_path_nor.png", "resources/textures/grass_ground");
    m_grassHeightMap = Render::TextureFromFile("grass_path_disp.png", "resources/textures/grass_ground");
    m_cobblestoneDiffMap = Render::TextureFromFile("cobblestone_diff.png", "resources/textures/cobblestone_ground");
    m_cobblestoneSpecMap = Render::TextureFromFile("cobblestone_spec.png", "resources/textures/cobblestone_ground");
    m_cobblestoneNormMap = Render::TextureFromFile("cobblestone_nor.png", "resources/textures/cobblestone_ground");
    m_cobblestoneHeightMap = Render::TextureFromFile("cobblestone_disp.png", "resources/textures/cobblestone_ground");

    m_shader->ActivateShader();
    m_shader->SetInt("material.texture_diffuse1", 0);
    m_shader->SetInt("material.texture_specular1", 1);
    m_shader->DeactivateShader();

    m_normalMapShader->ActivateShader();
    m_normalMapShader->SetInt("material.texture_diffuse1", 0);
    m_normalMapShader->SetInt("material.texture_specular1", 1);
    m_normalMapShader->SetInt("material.texture_normal1", 2);
    m_normalMapShader->DeactivateShader();

    m_normalAndHeightMapShader->ActivateShader();
    m_normalAndHeightMapShader->SetInt("material.texture_diffuse1", 0);
    m_normalAndHeightMapShader->SetInt("material.texture_specular1", 1);
    m_normalAndHeightMapShader->SetInt("material.texture_normal1", 2);
    m_normalAndHeightMapShader->SetInt("material.texture_height1", 3);
    m_normalAndHeightMapShader->DeactivateShader();
}

Entities::Ground::~Ground() {
    glDeleteVertexArrays(1, &m_grassVAO);
    glDeleteBuffers(1, &m_grassVBO);
    glDeleteVertexArrays(1, &m_cobblestoneVAO);
    glDeleteBuffers(1, &m_cobblestoneVBO);
}

void Entities::Ground::Update() {
    glCullFace(GL_FRONT);
    if (ProgramState::parallaxMapping)
        m_UpdateShader(m_normalAndHeightMapShader);
    else if (ProgramState::normalMapping)
        m_UpdateShader(m_normalMapShader);
    else
        m_UpdateShader(m_shader);
    glCullFace(GL_BACK);
}

void Entities::Ground::m_UpdateShader(const std::shared_ptr<Render::Shader> &shader) {
    shader->ActivateShader();

    view = Render::Camera::GetInstance().GetViewMatrix();
    shader->SetMat4("view", view);

    projection = glm::mat4(glm::perspective(glm::radians(Render::Camera::GetInstance().GetZoom()),
                                            static_cast<float>(Data::WindowData::screenWidth) / static_cast<float>(Data::WindowData::screenHeight),
                                            0.1f, 100.0f));
    shader->SetMat4("projection", projection);

    model = glm::mat4(1.0f);
    shader->SetMat4("model", model);

    if (ProgramState::parallaxMapping)
        shader->SetFloat("heightScale", 0.1f);

    // Forest ground
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_grassDiffMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_grassSpecMap);
    if (ProgramState::normalMapping) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_grassNormMap);
    }
    if (ProgramState::parallaxMapping) {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, m_grassHeightMap);
    }
    glBindVertexArray(m_grassVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Cobblestone ground
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_cobblestoneDiffMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_cobblestoneSpecMap);
    if (ProgramState::normalMapping) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_cobblestoneNormMap);
    }
    if (ProgramState::parallaxMapping) {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, m_cobblestoneHeightMap);
    }
    glBindVertexArray(m_cobblestoneVAO);
    glDrawArrays(GL_TRIANGLES, 0, 12);
    glBindVertexArray(0);

    shader->DeactivateShader();
}

void Entities::Ground::m_SetupGrassGround() {
    glm::vec3 pos1(15.0f,  0.0f, -15.0f);
    glm::vec3 pos2(15.0f, 0.0f, 15.0f);
    glm::vec3 pos3(-15.0f, 0.0f, 15.0f);
    glm::vec3 pos4(-15.0f,  0.0f, -15.0f);

    glm::vec2 uv1(10.0f, 10.0f);
    glm::vec2 uv2(10.0f, 0.0f);
    glm::vec2 uv3(0.0f, 0.0f);
    glm::vec2 uv4(0.0f, 10.0f);

    glm::vec3 nm(0.0f, 1.0f, 0.0f);

    glm::vec3 tangent1, bitangent1;
    glm::vec3 tangent2, bitangent2;

    // triangle 1
    glm::vec3 edge1 = pos2 - pos1;
    glm::vec3 edge2 = pos3 - pos1;
    glm::vec2 deltaUV1 = uv2 - uv1;
    glm::vec2 deltaUV2 = uv3 - uv1;

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent1 = glm::normalize(tangent1);

    bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent1 = glm::normalize(bitangent1);

    // triangle 2
    edge1 = pos3 - pos1;
    edge2 = pos4 - pos1;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent2 = glm::normalize(tangent2);

    bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent2 = glm::normalize(bitangent2);

    float vertices[] = {
            // positions            // normal         // texcoords  // tangent                          // bitangent
            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
    };
    // configure VAO
    glGenVertexArrays(1, &m_grassVAO);
    glGenBuffers(1, &m_grassVBO);

    glBindVertexArray(m_grassVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_grassVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));

    glBindVertexArray(0);
}

void Entities::Ground::m_SetupCobblestoneGround() {
    glm::vec3 pos1(5.0f,  0.01f, -5.0f);
    glm::vec3 pos2(5.0f, 0.01f, 5.0f);
    glm::vec3 pos3(-5.0f, 0.01f, 5.0f);
    glm::vec3 pos4(-5.0f,  0.01f, -5.0f);
    glm::vec3 pos5(2.0f,  0.01f, -13.0f);
    glm::vec3 pos6(2.0f, 0.01f, -5.0f);
    glm::vec3 pos7(-2.0f, 0.01f, -5.0f);
    glm::vec3 pos8(-2.0f,  0.01f, -13.0f);

    glm::vec2 uv1(10.0f, 10.0f);
    glm::vec2 uv2(10.0f, 0.0f);
    glm::vec2 uv3(0.0f, 0.0f);
    glm::vec2 uv4(0.0f, 10.0f);
    glm::vec2 uv5(4.0f, 8.0f);
    glm::vec2 uv6(4.0f, 0.0f);
    glm::vec2 uv7(0.0f, 0.0f);
    glm::vec2 uv8(0.0f, 8.0f);

    glm::vec3 nm(0.0f, 1.0f, 0.0f);

    glm::vec3 tangent1, bitangent1;
    glm::vec3 tangent2, bitangent2;
    glm::vec3 tangent3, bitangent3;
    glm::vec3 tangent4, bitangent4;

    // triangle 1
    glm::vec3 edge1 = pos2 - pos1;
    glm::vec3 edge2 = pos3 - pos1;
    glm::vec2 deltaUV1 = uv2 - uv1;
    glm::vec2 deltaUV2 = uv3 - uv1;

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent1 = glm::normalize(tangent1);

    bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent1 = glm::normalize(bitangent1);

    // triangle 2
    edge1 = pos3 - pos1;
    edge2 = pos4 - pos1;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent2 = glm::normalize(tangent2);

    bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent2 = glm::normalize(bitangent2);

    // triangle 3
    edge1 = pos6 - pos5;
    edge2 = pos7 - pos5;
    deltaUV1 = uv6 - uv5;
    deltaUV2 = uv7 - uv5;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent3.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent3.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent3.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent3 = glm::normalize(tangent3);

    bitangent3.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent3.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent3.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent3 = glm::normalize(bitangent3);

    // triangle 4
    edge1 = pos7 - pos5;
    edge2 = pos8 - pos5;
    deltaUV1 = uv7 - uv5;
    deltaUV2 = uv8 - uv5;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent4.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent4.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent4.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent4 = glm::normalize(tangent4);

    bitangent4.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent4.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent4.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent4 = glm::normalize(bitangent4);

    float vertices[] = {
            // positions            // normal         // texcoords  // tangent                          // bitangent
            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,

            pos5.x, pos5.y, pos5.z, nm.x, nm.y, nm.z, uv5.x, uv5.y, tangent3.x, tangent3.y, tangent3.z, bitangent3.x, bitangent3.y, bitangent3.z,
            pos6.x, pos6.y, pos6.z, nm.x, nm.y, nm.z, uv6.x, uv6.y, tangent3.x, tangent3.y, tangent3.z, bitangent3.x, bitangent3.y, bitangent3.z,
            pos7.x, pos7.y, pos7.z, nm.x, nm.y, nm.z, uv7.x, uv7.y, tangent3.x, tangent3.y, tangent3.z, bitangent3.x, bitangent3.y, bitangent3.z,

            pos5.x, pos5.y, pos5.z, nm.x, nm.y, nm.z, uv5.x, uv5.y, tangent4.x, tangent4.y, tangent4.z, bitangent4.x, bitangent4.y, bitangent4.z,
            pos7.x, pos7.y, pos7.z, nm.x, nm.y, nm.z, uv7.x, uv7.y, tangent4.x, tangent4.y, tangent4.z, bitangent4.x, bitangent4.y, bitangent4.z,
            pos8.x, pos8.y, pos8.z, nm.x, nm.y, nm.z, uv8.x, uv8.y, tangent4.x, tangent4.y, tangent4.z, bitangent4.x, bitangent4.y, bitangent4.z
    };

    // configure VAO
    glGenVertexArrays(1, &m_cobblestoneVAO);
    glGenBuffers(1, &m_cobblestoneVBO);

    glBindVertexArray(m_cobblestoneVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_cobblestoneVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));

    glBindVertexArray(0);
}
