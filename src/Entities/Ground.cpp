#include "Entities/Ground.h"

#include <utility>

#include <glad/glad.h>

#include "Model/Model.h"

Entities::Ground::Ground(std::shared_ptr<Render::Shader> shader) : m_shader(std::move(shader)) {
    m_SetupForestGround();
    m_SetupCobblestoneGround();

    m_forestDiffMap = Render::TextureFromFile("forest_diff.png", "resources/textures/forest_ground");
    m_forestSpecMap = Render::TextureFromFile("forest_spec.jpg", "resources/textures/forest_ground");
    m_cobblestoneDiffMap = Render::TextureFromFile("cobblestone_diff.png", "resources/textures/cobblestone_ground");
    m_cobblestoneSpecMap = Render::TextureFromFile("cobblestone_spec.png", "resources/textures/cobblestone_ground");
}

Entities::Ground::~Ground() {
    glDeleteVertexArrays(1, &m_forestVAO);
    glDeleteBuffers(1, &m_forestVBO);
    glDeleteBuffers(1, &m_forestEBO);
    glDeleteVertexArrays(1, &m_cobblestoneVAO);
    glDeleteBuffers(1, &m_cobblestoneVBO);
    glDeleteBuffers(1, &m_cobblestoneEBO);
}

void Entities::Ground::Update() {
    glCullFace(GL_FRONT);
    m_shader->ActivateShader();

    model = glm::mat4(1.0f);
    m_shader->SetMat4("model", model);

    // Forest ground
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_forestDiffMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_forestSpecMap);
    glBindVertexArray(m_forestVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    // Cobblestone ground
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_cobblestoneDiffMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_cobblestoneSpecMap);
    glBindVertexArray(m_cobblestoneVAO);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);

    m_shader->DeactivateShader();
    glCullFace(GL_BACK);
}

void Entities::Ground::m_SetupForestGround() {
    float groundVertices[] = {
            // positions                           // normals                     // texture coords
            10.0f,  0.0f, -15.0f, 0.0f, 1.0f, 0.0f,   50.0f, 50.0f, // top right
            10.0f, 0.0f, 10.0f, 0.0f, 1.0f, 0.0f,  50.0f, 0.0f, // bottom right
            -10.0f, 0.0f, 10.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bottom left
            -10.0f,  0.0f, -15.0f, 0.0f, 1.0f, 0.0f,  0.0f, 50.0f  // top left
    };
    unsigned int groundIndices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &m_forestVAO);
    glGenBuffers(1, &m_forestVBO);
    glGenBuffers(1, &m_forestEBO);

    glBindVertexArray(m_forestVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_forestVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_forestEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(groundIndices), groundIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));

    glBindVertexArray(0);
}

void Entities::Ground::m_SetupCobblestoneGround() {
    float groundVertices[] = {
            // positions                           // normals                     // texture coords
            // center square
            5.0f,  0.01f, -5.0f, 0.0f, 1.0f, 0.0f,   10.0f, 10.0f, // top right
            5.0f, 0.01f, 5.0f, 0.0f, 1.0f, 0.0f,  10.0f, 0.0f, // bottom right
            -5.0f, 0.01f, 5.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bottom left
            -5.0f,  0.01f, -5.0f, 0.0f, 1.0f, 0.0f,  0.0f, 10.0f,  // top left
            // path
            2.0f,  0.01f, -13.0f, 0.0f, 1.0f, 0.0f,   4.0f, 8.0f, // top right
            2.0f, 0.01f, -5.0f, 0.0f, 1.0f, 0.0f,  4.0f, 0.0f, // bottom right
            -2.0f, 0.01f, -5.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bottom left
            -2.0f,  0.01f, -13.0f, 0.0f, 1.0f, 0.0f,  0.0f, 8.0f  // top left
    };
    unsigned int groundIndices[] = {
            // center square
            0, 1, 3, // first triangle
            1, 2, 3,  // second triangle
            // path
            4, 5, 7, // first triangle
            5, 6, 7  // second triangle
    };

    glGenVertexArrays(1, &m_cobblestoneVAO);
    glGenBuffers(1, &m_cobblestoneVBO);
    glGenBuffers(1, &m_cobblestoneEBO);

    glBindVertexArray(m_cobblestoneVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_cobblestoneVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cobblestoneEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(groundIndices), groundIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));

    glBindVertexArray(0);
}
