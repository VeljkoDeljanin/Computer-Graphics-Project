#pragma once

#include "Entity.h"

namespace Entities {
    class Light : public Entity {
    public:

        explicit Light(std::shared_ptr<Render::Shader> shader, std::shared_ptr<Render::Shader> shader2,
                       std::shared_ptr<Render::Shader> shader3, std::shared_ptr<Render::Shader> shader4);
        ~Light() override = default;

        void Update() override;
    private:
        std::shared_ptr<Render::Shader> m_shader;
        std::shared_ptr<Render::Shader> m_normalMapShader;
        std::shared_ptr<Render::Shader> m_normalAndHeightMapShader;
        std::shared_ptr<Render::Shader> m_instancingShader;

        static inline bool m_hasNormalMap = false;

        std::array<glm::vec3, 6> m_pointLightPositions {
            glm::vec3(-1.7f, 2.4f, -8.0f),
            glm::vec3(1.7f, 2.4f, -11.0f),
            glm::vec3(4.5f, 2.4f, 4.5f),
            glm::vec3(4.5f, 2.4f, -4.5f),
            glm::vec3(-4.5f, 2.4f, 4.5f),
            glm::vec3(-4.5f, 2.4f, -4.5f)
        };

        std::array<glm::vec3, 4> m_billboardLightPositions {
            glm::vec3(-3.25f, 0.0f, 0.1f),
            glm::vec3(-3.25f, 0.0f, -0.1f),
            glm::vec3(3.25f, 0.0f, 0.1f),
            glm::vec3(3.25f, 0.0f, -0.1f)
        };

        std::array<glm::vec3, 4> m_billboardLightsDirections {
            glm::vec3(1.0f, 0.0f, -1.0f),
            glm::vec3(1.0f, 0.0f, 1.0f),
            glm::vec3(-1.0f, 0.0f, -1.0f),
            glm::vec3(-1.0f, 0.0f, 1.0f)
        };

        void m_UpdateShader(const std::shared_ptr<Render::Shader>& shader);

        static void m_UpdateDirLight(const std::shared_ptr<Render::Shader>& shader);
        void m_UpdatePointsLights(const std::shared_ptr<Render::Shader>& shader) const;
        void m_UpdateSpotLight(const std::shared_ptr<Render::Shader>& shader) const;
    };
}
