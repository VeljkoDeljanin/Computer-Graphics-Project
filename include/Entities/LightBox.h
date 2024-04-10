#pragma once

#include <vector>

#include "Entity.h"

namespace Entities {
    class LightBox : public Entity {
    public:
        explicit LightBox(std::shared_ptr<Render::Shader> shader);
        ~LightBox() override;

        void Update() override;
    private:
        std::shared_ptr<Render::Shader> m_shader;

        unsigned int VAO{}, VBO{};

        glm::mat4 model{};
        glm::mat4 view{};
        glm::mat4 projection{};

        glm::vec3 m_lightColor{};

        std::vector<unsigned int> m_textures;

        std::array<glm::vec3, 4> m_lightBoxPositions {
            glm::vec3(-4.0f, 1.05f, 1.0f),
            glm::vec3(-4.0f, 1.05f, -1.0f),
            glm::vec3(4.0f, 1.025f, 1.0f),
            glm::vec3(4.0f, 1.05f, -1.0f)
        };

        std::array<glm::vec3, 4> m_lightBoxScales {
            glm::vec3(0.01f, 0.6f, 0.4f),
            glm::vec3(0.01f, 0.6f, 0.4f),
            glm::vec3(0.01f, 0.52f, 0.4f),
            glm::vec3(0.01f, 0.71f, 0.4f)
        };
    };
}
