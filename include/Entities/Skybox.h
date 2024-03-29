#pragma once

#include "Entity.h"

namespace Entities {
    class Skybox : public Entity {
    public:
        explicit Skybox(std::shared_ptr<Render::Shader> shader);
        ~Skybox() override;

        void Update() override;

    private:
        unsigned int VAO{}, VBO{};
        unsigned int m_textureID{};

        glm::mat4 projection;
        glm::mat4 view{};

        static unsigned int m_LoadCubemap();
    };
}
