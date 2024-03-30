#include "Entities/Light.h"

#include "Camera.h"
#include "ProgramState.h"

Entities::Light::Light(std::shared_ptr<Render::Shader> shader) {
    m_shader = std::move(shader);

    // Directional light
    m_dirLight.direction = glm::vec3(-0.35f, -0.6f, -1.0f);

    m_dirLight.ambient = glm::vec3(0.15f);
    m_dirLight.diffuse = glm::vec3(0.1f);
    m_dirLight.specular = glm::vec3(1.0f);

    // Spotlight
    m_spotLight.position = Render::Camera::GetInstance().GetPosition();
    m_spotLight.direction = Render::Camera::GetInstance().GetFront();
    m_spotLight.cutOff = glm::cos(glm::radians(10.0f));
    m_spotLight.outerCutOff = glm::cos(glm::radians(15.0f));

    m_spotLight.constant = 1.0f;
    m_spotLight.linear = 0.09f;
    m_spotLight.quadratic = 0.032f;

    m_spotLight.ambient = glm::vec3(0.0f);
    m_spotLight.diffuseOn = glm::vec3(1.0f);
    m_spotLight.specularOn = glm::vec3(0.5f);
    m_spotLight.diffuseOff = glm::vec3(0.0f);
    m_spotLight.specularOff = glm::vec3(0.0f);

    m_shininess = 32.0f;
}

void Entities::Light::Update() {
    m_shader->ActivateShader();

    m_shader->SetVec3("viewPosition", Render::Camera::GetInstance().GetPosition());
    m_shader->SetFloat("material.shininess", m_shininess);

    m_UpdateDirLight();
    m_UpdateSpotLight();

    m_shader->DeactivateShader();
}

void Entities::Light::m_UpdateDirLight() {
    m_shader->SetVec3("dirLight.direction", m_dirLight.direction);

    m_shader->SetVec3("dirLight.ambient", m_dirLight.ambient);
    m_shader->SetVec3("dirLight.diffuse", m_dirLight.diffuse);
    m_shader->SetVec3("dirLight.specular", m_dirLight.specular);
}

void Entities::Light::m_UpdateSpotLight() {
    m_shader->SetVec3("spotLight.position", Render::Camera::GetInstance().GetPosition());
    m_shader->SetVec3("spotLight.direction", Render::Camera::GetInstance().GetFront());
    m_shader->SetFloat("spotLight.cutOff", m_spotLight.cutOff);
    m_shader->SetFloat("spotLight.outerCutOff", m_spotLight.outerCutOff);

    m_shader->SetFloat("spotLight.constant", m_spotLight.constant);
    m_shader->SetFloat("spotLight.linear", m_spotLight.linear);
    m_shader->SetFloat("spotLight.quadratic", m_spotLight.quadratic);

    m_shader->SetVec3("spotLight.ambient", m_spotLight.ambient);
    if (ProgramState::flashlightOn) {
        m_shader->SetVec3("spotLight.diffuse", m_spotLight.diffuseOn);
        m_shader->SetVec3("spotLight.specular", m_spotLight.specularOn);
    }
    else {
        m_shader->SetVec3("spotLight.diffuse", m_spotLight.diffuseOff);
        m_shader->SetVec3("spotLight.specular", m_spotLight.specularOff);
    }
}
