#include "Entities/Model3D.h"

#include "Camera.h"
#include "ProgramState.h"
#include "Data.h"

Entities::Model3D::Model3D(std::shared_ptr<Render::Shader> shader, std::shared_ptr<Render::Shader> shader2,
                           std::shared_ptr<Render::Shader> shader3)
: m_shader(std::move(shader)), m_normalMapShader(std::move(shader2)), m_instancingShader(std::move(shader3)),
  m_flashlight("resources/objects/flashlight/newer.obj"),
  m_fountain("resources/objects/fountain/scene.gltf"),
  m_tableChairSet("resources/objects/table_chair_set/outdoor_table_chair_set_01_4k.gltf"),
  m_streetLamp("resources/objects/street_lamp/Street_Lamp_7.obj"),
  m_parkBench("resources/objects/park_bench/scene.gltf"),
  m_billboard("resources/objects/billboard/3d-model.obj"),
  m_telescope("resources/objects/telescope/scene.gltf"),
  m_chessSet("resources/objects/chess_set/untitled.obj"),
  m_tree("resources/objects/tree/tree.obj") {

    m_flashlight.SetShaderTextureNamePrefix("material.");
    m_fountain.SetShaderTextureNamePrefix("material.");
    m_tableChairSet.SetShaderTextureNamePrefix("material.");
    m_streetLamp.SetShaderTextureNamePrefix("material.");
    m_parkBench.SetShaderTextureNamePrefix("material.");
    m_billboard.SetShaderTextureNamePrefix("material.");
    m_telescope.SetShaderTextureNamePrefix("material.");
    m_chessSet.SetShaderTextureNamePrefix("material.");
    m_tree.SetShaderTextureNamePrefix("material.");

    m_instancingShader->ActivateShader();
    m_instancingShader->SetInt("material.texture_diffuse1", 0);
    m_instancingShader->SetInt("material.texture_specular1", 1);
    m_instancingShader->DeactivateShader();

    m_SetupInstancing();
}

Entities::Model3D::~Model3D() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &buffer);
    delete[] modelMatrices;
}

void Entities::Model3D::Update() {
    m_hasNormalMap = false;
    m_UpdateShader(m_shader);
    m_hasNormalMap = true;
    m_UpdateShader(m_normalMapShader);
    m_UpdateInstancedTrees();
}

void Entities::Model3D::m_UpdateShader(const std::shared_ptr<Render::Shader> &shader) {
    shader->ActivateShader();

    view = Render::Camera::GetInstance().GetViewMatrix();
    shader->SetMat4("view", view);

    projection = glm::mat4(glm::perspective(glm::radians(Render::Camera::GetInstance().GetZoom()),
                                            static_cast<float>(Data::WindowData::screenWidth) / static_cast<float>(Data::WindowData::screenHeight),
                                            0.1f, 100.0f));
    shader->SetMat4("projection", projection);

    if (!m_hasNormalMap) {
        m_UpdateFountain(shader);
        m_UpdateTableChairSet(shader);
        m_UpdateStreetLamp(shader);
        m_UpdateParkBench(shader);
        m_UpdateBillboard(shader);
        m_UpdateTelescope(shader);
        m_UpdateChessSet(shader);

        if (!ProgramState::normalMapping)
            m_UpdateFlashlight(shader);
    }

    if (m_hasNormalMap && ProgramState::normalMapping)
        m_UpdateFlashlight(shader);

    shader->DeactivateShader();
}

void Entities::Model3D::m_UpdateFlashlight(const std::shared_ptr<Render::Shader> &shader) {
    Render::Camera &camera = Render::Camera::GetInstance();

    model = glm::mat4(1.0f);

    model = glm::translate(model,
                           camera.GetPosition() + 0.5f * camera.GetFront() + 0.275f * camera.GetRight() - 0.15f * camera.GetUp());
    model = glm::rotate(model, -glm::radians(camera.GetYaw()), camera.GetUp());
    model = glm::rotate(model, glm::radians(camera.GetPitch()), camera.GetRight());
    model = glm::scale(model, glm::vec3(0.01f));

    shader->SetMat4("model", model);
    m_flashlight.Draw(*shader);
}

void Entities::Model3D::m_UpdateFountain(const std::shared_ptr<Render::Shader> &shader) {
    model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.004f));

    shader->SetMat4("model", model);
    m_fountain.Draw(*shader);
}

void Entities::Model3D::m_UpdateTableChairSet(const std::shared_ptr<Render::Shader> &shader) {
    for (auto &m_chessTablePosition : m_chessTablePositions) {
        model = glm::mat4(1.0f);

        model = glm::translate(model, m_chessTablePosition);
        model = glm::scale(model, glm::vec3(0.9f));

        shader->SetMat4("model", model);
        m_tableChairSet.Draw(*shader);
    }
}

void Entities::Model3D::m_UpdateStreetLamp(const std::shared_ptr<Render::Shader> &shader) {
    for (auto &m_streetLampPosition : m_streetLampPositions) {
        model = glm::mat4(1.0f);

        model = glm::translate(model, m_streetLampPosition);
        model = glm::scale(model, glm::vec3(1.0f));

        shader->SetMat4("model", model);
        m_streetLamp.Draw(*shader);
    }
}

void Entities::Model3D::m_UpdateParkBench(const std::shared_ptr<Render::Shader> &shader) {
    model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(1.5f, 0.36f, -9.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.8f));

    shader->SetMat4("model", model);
    m_parkBench.Draw(*shader);

    model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(-1.5f, 0.36f, -6.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.8f));

    shader->SetMat4("model", model);
    m_parkBench.Draw(*shader);
}

void Entities::Model3D::m_UpdateBillboard(const std::shared_ptr<Render::Shader> &shader) {
    model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(-4.5f, 0.01f, 0.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.013f));

    shader->SetMat4("model", model);
    m_billboard.Draw(*shader);

    model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(4.5f, 0.01f, 0.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.013f));

    shader->SetMat4("model", model);
    m_billboard.Draw(*shader);
}

void Entities::Model3D::m_UpdateTelescope(const std::shared_ptr<Render::Shader> &shader) {
    model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(0.0f, 0.91f, 4.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, glm::radians(-19.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.005f));

    shader->SetMat4("model", model);
    m_telescope.Draw(*shader);
}

void Entities::Model3D::m_UpdateChessSet(const std::shared_ptr<Render::Shader> &shader) {
    for (auto &m_chessSetPosition : m_chessSetPositions) {
        model = glm::mat4(1.0f);

        model = glm::translate(model, m_chessSetPosition);
        model = glm::scale(model, glm::vec3(1.0f));

        shader->SetMat4("model", model);
        m_chessSet.Draw(*shader);
    }
}

void Entities::Model3D::m_SetupInstancing() {
    modelMatrices = new glm::mat4[Data::TreeData::numOfTrees];
    for (unsigned int i = 0; i < Data::TreeData::numOfTrees; i++) {
        model = glm::mat4(1.0f);

        model = glm::translate(model, Data::TreeData::treePositions[i]);
        model = glm::scale(model, glm::vec3(0.01f));

        modelMatrices[i] = model;
    }

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(Data::TreeData::numOfTrees * sizeof(glm::mat4)), &modelMatrices[0], GL_STATIC_DRAW);

    for (auto &mesh : m_tree.meshes) {
        VAO = mesh.VAO;
        glBindVertexArray(VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)nullptr);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }
}

void Entities::Model3D::m_UpdateInstancedTrees() {
    m_instancingShader->ActivateShader();

    view = Render::Camera::GetInstance().GetViewMatrix();
    m_instancingShader->SetMat4("view", view);

    projection = glm::mat4(glm::perspective(glm::radians(Render::Camera::GetInstance().GetZoom()),
                                            static_cast<float>(Data::WindowData::screenWidth) / static_cast<float>(Data::WindowData::screenHeight),
                                            0.1f, 100.0f));
    m_instancingShader->SetMat4("projection", projection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_tree.loadedTexturesMap["Leavs_baseColor.png"].id);
    glBindVertexArray(m_tree.meshes[0].VAO);
    glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(m_tree.meshes[0].indices.size()),
                            GL_UNSIGNED_INT, nullptr, static_cast<GLsizei>(Data::TreeData::numOfTrees));
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_tree.loadedTexturesMap["Trank_baseColor.png"].id);
    glBindVertexArray(m_tree.meshes[1].VAO);
    glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(m_tree.meshes[1].indices.size()),
                            GL_UNSIGNED_INT, nullptr, static_cast<GLsizei>(Data::TreeData::numOfTrees));
    glBindVertexArray(0);

    m_instancingShader->DeactivateShader();
}
