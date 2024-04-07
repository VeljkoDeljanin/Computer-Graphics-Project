#include "Entities/Light.h"

#include "Camera.h"
#include "ProgramState.h"

Entities::Light::Light(std::shared_ptr<Render::Shader> shader, std::shared_ptr<Render::Shader> shader2,
                       std::shared_ptr<Render::Shader> shader3, std::shared_ptr<Render::Shader> shader4)
: m_shader(std::move(shader)), m_normalMapShader(std::move(shader2)),
  m_normalAndHeightMapShader(std::move(shader3)), m_instancingShader(std::move(shader4)) {

}

void Entities::Light::Update() {
    m_hasNormalMap = false;
    m_UpdateShader(m_shader);
    m_UpdateShader(m_instancingShader);
    m_hasNormalMap = true;
    m_UpdateShader(m_normalMapShader);
    m_UpdateShader(m_normalAndHeightMapShader);
}

void Entities::Light::m_UpdateShader(const std::shared_ptr<Render::Shader>& shader) {
    shader->ActivateShader();

    shader->SetVec3("viewPosition", Render::Camera::GetInstance().GetPosition());
    shader->SetFloat("material.shininess", 32.0f);

    if (m_hasNormalMap)
        shader->SetVec3("viewPos", Render::Camera::GetInstance().GetPosition());

    m_UpdateDirLight(shader);
    m_UpdatePointsLights(shader);
    m_UpdateSpotLight(shader);

    shader->DeactivateShader();
}

void Entities::Light::m_UpdateDirLight(const std::shared_ptr<Render::Shader>& shader) {
    if (m_hasNormalMap)
        shader->SetVec3("lightDir[0]", glm::vec3(-0.35f, -0.6f, -1.0f));

    shader->SetVec3("dirLight.direction", glm::vec3(-0.35f, -0.6f, -1.0f));

    shader->SetVec3("dirLight.ambient", glm::vec3(0.1f));
    shader->SetVec3("dirLight.diffuse", glm::vec3(0.1f));
    shader->SetVec3("dirLight.specular", glm::vec3(1.0f));
}

void Entities::Light::m_UpdatePointsLights(const std::shared_ptr<Render::Shader> &shader) const {
    for (unsigned int i = 0; i < m_pointLightPositions.size(); i++) {
        if (m_hasNormalMap)
            shader->SetVec3("lightPos[" + std::to_string(i) + "]", m_pointLightPositions[i]);

        shader->SetVec3("pointLights[" + std::to_string(i) + "].position", m_pointLightPositions[i]);

        shader->SetFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
        shader->SetFloat("pointLights[" + std::to_string(i) + "].linear", 0.35f);
        shader->SetFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.44f);

        shader->SetVec3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0.5f));
        shader->SetVec3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0.8f));
        shader->SetVec3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(1.0f));
    }
}

void Entities::Light::m_UpdateSpotLight(const std::shared_ptr<Render::Shader>& shader) {
    if (m_hasNormalMap) {
        shader->SetVec3("lightPos[6]", Render::Camera::GetInstance().GetPosition());
        shader->SetVec3("lightDir[1]", Render::Camera::GetInstance().GetFront());
    }

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
