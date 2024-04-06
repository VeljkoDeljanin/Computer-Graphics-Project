#include "Entities/Model3D.h"

#include "Camera.h"

Entities::Model3D::Model3D(std::shared_ptr<Render::Shader> shader, std::shared_ptr<Render::Shader> shader2)
: m_shader(std::move(shader)), m_normalMapShader(std::move(shader2)),
  m_flashlight("resources/objects/flashlight/newer.obj"),
  m_bigChessSet("resources/objects/chess/scene.gltf"),
  m_tableChairSet("resources/objects/table_chair_set/outdoor_table_chair_set_01_4k.gltf"),
  m_streetLamp("resources/objects/street_lamp/Street_Lamp_7.obj"),
  m_parkBench("resources/objects/park_bench/scene.gltf"),
  m_billboard("resources/objects/billboard/3d-model.obj"),
  m_telescope("resources/objects/telescope/scene.gltf"),
  m_chessSet("resources/objects/chess_set/untitled.obj") {

    m_flashlight.SetShaderTextureNamePrefix("material.");
    m_bigChessSet.SetShaderTextureNamePrefix("material.");
    m_tableChairSet.SetShaderTextureNamePrefix("material.");
    m_streetLamp.SetShaderTextureNamePrefix("material.");
    m_parkBench.SetShaderTextureNamePrefix("material.");
    m_billboard.SetShaderTextureNamePrefix("material.");
    m_telescope.SetShaderTextureNamePrefix("material.");
    m_chessSet.SetShaderTextureNamePrefix("material.");
}

void Entities::Model3D::Update() {
    m_hasNormalMap = false;
    m_UpdateShader(m_shader);
    m_hasNormalMap = true;
    m_UpdateShader(m_normalMapShader);
}

void Entities::Model3D::m_UpdateShader(const std::shared_ptr<Render::Shader> &shader) {
    shader->ActivateShader();

    view = Render::Camera::GetInstance().GetViewMatrix();
    shader->SetMat4("view", view);

    projection = glm::mat4(glm::perspective(glm::radians(Render::Camera::GetInstance().GetZoom()),
                                            static_cast<float>(Data::WindowData::screenWidth) / static_cast<float>(Data::WindowData::screenHeight),
                                            0.1f, 100.0f));
    shader->SetMat4("projection", projection);

    if (m_hasNormalMap) {
        m_UpdateFlashlight(shader);
    }
    else {
        m_UpdateBigChessSet(shader);
        m_UpdateTableChairSet(shader);
        m_UpdateStreetLamp(shader);
        m_UpdateParkBench(shader);
        m_UpdateBillboard(shader);
        m_UpdateTelescope(shader);
        m_UpdateChessSet(shader);
    }

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

void Entities::Model3D::m_UpdateBigChessSet(const std::shared_ptr<Render::Shader> &shader) {
    model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.08f));

    shader->SetMat4("model", model);
    m_bigChessSet.Draw(*shader);
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

    model = glm::translate(model, glm::vec3(1.5f, 0.36f, -10.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.8f));

    shader->SetMat4("model", model);
    m_parkBench.Draw(*shader);

    model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(-1.5f, 0.36f, -7.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.8f));

    shader->SetMat4("model", model);
    m_parkBench.Draw(*shader);
}

void Entities::Model3D::m_UpdateBillboard(const std::shared_ptr<Render::Shader> &shader) {
    model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(-4.5f, 0.01f, 0.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.015f));

    shader->SetMat4("model", model);
    m_billboard.Draw(*shader);

    model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(4.5f, 0.01f, 0.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.015f));

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
