#pragma once

#include "Entity.h"

namespace Entities {
    class Ground : public Entity {
    public:
        explicit Ground(std::shared_ptr<Render::Shader> shader);
        ~Ground() override;

        void Update() override;
    private:
        std::shared_ptr<Render::Shader> m_shader;

        glm::mat4 model{};
        glm::mat4 view{};
        glm::mat4 projection{};

        unsigned int m_grassVAO{}, m_grassVBO{};
        unsigned int m_cobblestoneVAO{}, m_cobblestoneVBO{};

        unsigned int m_grassDiffMap{}, m_grassSpecMap{}, m_grassNormMap{};
        unsigned int m_cobblestoneDiffMap{}, m_cobblestoneSpecMap{}, m_cobblestoneNormMap{};

        void m_SetupGrassGround();
        void m_SetupCobblestoneGround();
    };
}
