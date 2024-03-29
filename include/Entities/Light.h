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

        explicit Light(std::shared_ptr<Render::Shader> shader);
        ~Light() override = default;

        void Update() override;
    private:
        DirLight m_dirLight{};
        float m_shininess{};

        void m_UpdateDirLight();
    };
}
