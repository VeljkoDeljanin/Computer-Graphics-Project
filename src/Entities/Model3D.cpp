#include "Entities/Model3D.h"

#include "Camera.h"

Entities::Model3D::Model3D(std::shared_ptr<Render::Shader> shader)
: m_flashlight("resources/objects/flashlight/scene.gltf"),
  projection(glm::perspective(glm::radians(Render::Camera::GetInstance().GetZoom()),
                              static_cast<float>(Data::WindowData::screenWidth) / static_cast<float>(Data::WindowData::screenHeight),
                              0.1f, 100.0f)) {
    m_shader = std::move(shader);

    m_shader->ActivateShader();
    m_shader->SetMat4("projection", projection);
    m_shader->DeactivateShader();
}

void Entities::Model3D::Update() {
    m_shader->ActivateShader();

    view = Render::Camera::GetInstance().GetViewMatrix();
    m_shader->SetMat4("view", view);

    m_UpdateFlashlight();

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
