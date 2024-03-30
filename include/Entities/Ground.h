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

        unsigned int m_forestVAO{}, m_forestVBO{}, m_forestEBO{};
        unsigned int m_cobblestoneVAO{}, m_cobblestoneVBO{}, m_cobblestoneEBO{};

        unsigned int m_forestDiffMap{}, m_forestSpecMap{};
        unsigned int m_cobblestoneDiffMap{}, m_cobblestoneSpecMap{};

        void m_SetupForestGround();
        void m_SetupCobblestoneGround();
    };
}
