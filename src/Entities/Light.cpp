#include "Entities/Light.h"

#include "Camera.h"

Entities::Light::Light(std::shared_ptr<Render::Shader> shader) {
    m_shader = std::move(shader);

    m_dirLight.direction = glm::vec3(-0.35f, -0.6f, -1.0f);
    m_dirLight.ambient = glm::vec3(0.15f);
    m_dirLight.diffuse = glm::vec3(0.1f);
    m_dirLight.specular = glm::vec3(1.0f);

    m_shininess = 32.0f;
}

void Entities::Light::Update() {
    m_UpdateDirLight();
}

void Entities::Light::m_UpdateDirLight() {
    m_shader->ActivateShader();

    m_shader->SetVec3("viewPosition", Render::Camera::GetInstance().GetPosition());
    m_shader->SetFloat("material.shininess", m_shininess);

    m_shader->SetVec3("dirLight.direction", m_dirLight.direction);
    m_shader->SetVec3("dirLight.ambient", m_dirLight.ambient);
    m_shader->SetVec3("dirLight.diffuse", m_dirLight.diffuse);
    m_shader->SetVec3("dirLight.specular", m_dirLight.specular);

    m_shader->DeactivateShader();
}
