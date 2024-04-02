#include "Entities/Model3D.h"

#include "Camera.h"

Entities::Model3D::Model3D(std::shared_ptr<Render::Shader> shader)
: m_shader(std::move(shader)),
  m_flashlight("resources/objects/flashlight/scene.gltf"),
  m_chessBoard("resources/objects/chess_board/12951_Stone_Chess_Board_v1_L3.obj"),
  m_chessTable("resources/objects/chess_table/chess_table.obj"),
  m_streetLamp("resources/objects/street_lamp/Street_Lamp_7.obj"),
  m_parkBench("resources/objects/park_bench/scene.gltf"),
  m_billboard("resources/objects/billboard/3d-model.obj"),
  m_telescope("resources/objects/telescope/scene.gltf") {

    m_flashlight.SetShaderTextureNamePrefix("material.");
    m_chessBoard.SetShaderTextureNamePrefix("material.");
    m_chessTable.SetShaderTextureNamePrefix("material.");
    m_streetLamp.SetShaderTextureNamePrefix("material.");
    m_parkBench.SetShaderTextureNamePrefix("material.");
    m_billboard.SetShaderTextureNamePrefix("material.");
    m_telescope.SetShaderTextureNamePrefix("material.");
}

void Entities::Model3D::Update() {
    m_shader->ActivateShader();

    view = Render::Camera::GetInstance().GetViewMatrix();
    m_shader->SetMat4("view", view);

    projection = glm::mat4(glm::perspective(glm::radians(Render::Camera::GetInstance().GetZoom()),
                                            static_cast<float>(Data::WindowData::screenWidth) / static_cast<float>(Data::WindowData::screenHeight),
                                            0.1f, 100.0f));
    m_shader->SetMat4("projection", projection);

    m_UpdateFlashlight();
    m_UpdateChessBoard();
    m_UpdateChessTable();
    m_UpdateStreetLamp();
    m_UpdateParkBench();
    m_UpdateBillboard();
    m_UpdateTelescope();

    m_shader->DeactivateShader();
}

void Entities::Model3D::m_UpdateFlashlight() {
    Render::Camera &camera = Render::Camera::GetInstance();

    model = glm::mat4(1.0f);

    model = glm::translate(model,
                           camera.GetPosition() + 0.5f * camera.GetFront() + 0.25f * camera.GetRight() - 0.13f * camera.GetUp());
    model = glm::rotate(model, -glm::radians(camera.GetYaw()), camera.GetUp());
    model = glm::rotate(model, glm::radians(camera.GetPitch()), camera.GetRight());
    model = glm::scale(model, glm::vec3(0.15f));

    m_shader->SetMat4("model", model);
    m_flashlight.Draw(*m_shader);
}

void Entities::Model3D::m_UpdateChessBoard() {
    model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(0.0f, 0.16f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(0.08f));

    m_shader->SetMat4("model", model);
    m_chessBoard.Draw(*m_shader);
}

void Entities::Model3D::m_UpdateChessTable() {
    for (unsigned int i = 0; i < m_chessTablePositions.size(); i++) {
        model = glm::mat4(1.0f);

        model = glm::translate(model, m_chessTablePositions[i]);
        model = glm::rotate(model, m_chessTableRotations[i/2], glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.9f));

        m_shader->SetMat4("model", model);
        m_chessTable.Draw(*m_shader);
    }
}

void Entities::Model3D::m_UpdateStreetLamp() {
    for (auto &m_streetLampPosition : m_streetLampPositions) {
        model = glm::mat4(1.0f);

        model = glm::translate(model, m_streetLampPosition);
        model = glm::scale(model, glm::vec3(1.0f));

        m_shader->SetMat4("model", model);
        m_streetLamp.Draw(*m_shader);
    }
}

void Entities::Model3D::m_UpdateParkBench() {
    model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(1.5f, 0.36f, -10.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.8f));

    m_shader->SetMat4("model", model);
    m_parkBench.Draw(*m_shader);

    model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(-1.5f, 0.36f, -7.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.8f));

    m_shader->SetMat4("model", model);
    m_parkBench.Draw(*m_shader);
}

void Entities::Model3D::m_UpdateBillboard() {
    model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(-4.5f, 0.01f, 0.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.015f));

    m_shader->SetMat4("model", model);
    m_billboard.Draw(*m_shader);

    model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(4.5f, 0.01f, 0.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.015f));

    m_shader->SetMat4("model", model);
    m_billboard.Draw(*m_shader);
}

void Entities::Model3D::m_UpdateTelescope() {
    model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(0.0f, 0.91f, 4.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, glm::radians(-19.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.005f));

    m_shader->SetMat4("model", model);
    m_telescope.Draw(*m_shader);
}
