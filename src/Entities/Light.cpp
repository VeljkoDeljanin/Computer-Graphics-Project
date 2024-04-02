#include "Entities/Light.h"

#include "Camera.h"
#include "ProgramState.h"

Entities::Light::Light(std::shared_ptr<Render::Shader> shader)
: m_shader(std::move(shader)) {

}

void Entities::Light::Update() {
    m_UpdateShader(m_shader);
}

void Entities::Light::m_UpdateShader(const std::shared_ptr<Render::Shader>& shader) {
    shader->ActivateShader();

    shader->SetVec3("viewPosition", Render::Camera::GetInstance().GetPosition());
    shader->SetFloat("material.shininess", 32.0f);

    m_UpdateDirLight(shader);
    m_UpdatePointsLights(shader);
    m_UpdateSpotLight(shader);

    shader->DeactivateShader();
}

void Entities::Light::m_UpdateDirLight(const std::shared_ptr<Render::Shader>& shader) {
    shader->SetVec3("dirLight.direction", glm::vec3(-0.35f, -0.6f, -1.0f));

    shader->SetVec3("dirLight.ambient", glm::vec3(0.1f));
    shader->SetVec3("dirLight.diffuse", glm::vec3(0.1f));
    shader->SetVec3("dirLight.specular", glm::vec3(1.0f));
}

void Entities::Light::m_UpdatePointsLights(const std::shared_ptr<Render::Shader> &shader) const {
    for (unsigned int i = 0; i < m_pointLightPositions.size(); i++) {
        shader->SetVec3("pointLights[" + std::to_string(i) + "].position", m_pointLightPositions[i]);

        shader->SetFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
        shader->SetFloat("pointLights[" + std::to_string(i) + "].linear", 0.7f);
        shader->SetFloat("pointLights[" + std::to_string(i) + "].quadratic", 1.8f);

        shader->SetVec3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0.5f));
        shader->SetVec3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0.8f));
        shader->SetVec3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(1.0f));
    }
}

void Entities::Light::m_UpdateSpotLight(const std::shared_ptr<Render::Shader>& shader) {
    shader->SetVec3("spotLight.position", Render::Camera::GetInstance().GetPosition());
    shader->SetVec3("spotLight.direction", Render::Camera::GetInstance().GetFront());
    shader->SetFloat("spotLight.cutOff", glm::cos(glm::radians(10.0f)));
    shader->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

    shader->SetFloat("spotLight.constant", 1.0f);
    shader->SetFloat("spotLight.linear", 0.09f);
    shader->SetFloat("spotLight.quadratic", 0.032f);

    shader->SetVec3("spotLight.ambient", glm::vec3(0.0f));
    if (ProgramState::flashlight) {
        shader->SetVec3("spotLight.diffuse", glm::vec3(1.0f));
        shader->SetVec3("spotLight.specular", glm::vec3(0.5f));
    }
    else {
        shader->SetVec3("spotLight.diffuse", glm::vec3(0.0f));
        shader->SetVec3("spotLight.specular", glm::vec3(0.0f));
    }
}
