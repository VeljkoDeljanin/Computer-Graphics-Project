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
        std::shared_ptr<Render::Shader> m_shader;

        glm::mat4 model{};
        glm::mat4 view{};
        glm::mat4 projection{};

        Render::Model m_flashlight;
        Render::Model m_chessBoard;
        Render::Model m_chessTable;

        std::array<glm::vec3, 4> m_chessTablePositions {
                glm::vec3(3.5f, 0.01f, -4.0f),
                glm::vec3(3.5f, 0.01f, 4.0f),
                glm::vec3(-3.5f, 0.01f, 4.0f),
                glm::vec3(-3.5f, 0.01f, -4.0f)
        };

        std::array<float, 2> m_chessTableRotations {
            glm::radians(0.0f),
            glm::radians(180.0f)
        };

        void m_UpdateFlashlight();
        void m_UpdateChessBoard();
        void m_UpdateChessTable();
    };
}
