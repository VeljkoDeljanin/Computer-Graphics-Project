#pragma once

#include "Entity.h"

namespace Entities {
    class Light : public Entity {
    public:
        struct DirLight {
            glm::vec3 direction;

            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
        };

        struct SpotLight {
            glm::vec3 position;
            glm::vec3 direction;
            float cutOff;
            float outerCutOff;

            float constant;
            float linear;
            float quadratic;

            glm::vec3 ambient;
            glm::vec3 diffuseOn;
            glm::vec3 specularOn;
            glm::vec3 diffuseOff;
            glm::vec3 specularOff;
        };

        explicit Light(std::shared_ptr<Render::Shader> shader);
        ~Light() override = default;

        void Update() override;
    private:
        std::shared_ptr<Render::Shader> m_shader;

        DirLight m_dirLight{};
        SpotLight m_spotLight{};

        float m_shininess{};

        void m_UpdateShader(const std::shared_ptr<Render::Shader>& shader);

        void m_UpdateDirLight(const std::shared_ptr<Render::Shader>& shader) const;
        void m_UpdateSpotLight(const std::shared_ptr<Render::Shader>& shader) const;
    };
}
