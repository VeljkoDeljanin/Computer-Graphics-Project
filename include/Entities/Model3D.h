#pragma once

#include "Entity.h"
#include "Model/Model.h"

namespace Entities {
    class Model3D : public Entity {
    public:
        explicit Model3D(std::shared_ptr<Render::Shader> shader);
        ~Model3D() override = default;

        void Update() override;
    private:
        Render::Model m_flashlight;

        glm::mat4 model{};
        glm::mat4 view{};
        glm::mat4 projection;

        void m_UpdateFlashlight();
    };
}
