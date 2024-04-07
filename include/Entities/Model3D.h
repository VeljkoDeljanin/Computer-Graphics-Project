#pragma once

#include "Entity.h"
#include "Model/Model.h"

namespace Entities {
    class Model3D : public Entity {
    public:
        explicit Model3D(std::shared_ptr<Render::Shader> shader, std::shared_ptr<Render::Shader> shader2,
                         std::shared_ptr<Render::Shader> shader3);
        ~Model3D() override;

        void Update() override;
    private:
        std::shared_ptr<Render::Shader> m_shader;
        std::shared_ptr<Render::Shader> m_normalMapShader;
        std::shared_ptr<Render::Shader> m_instancingShader;

        static inline bool m_hasNormalMap = false;

        glm::mat4 model{};
        glm::mat4 view{};
        glm::mat4 projection{};

        unsigned int buffer{}, VAO{};
        glm::mat4 *modelMatrices = nullptr;

        Render::Model m_flashlight;
        Render::Model m_fountain;
        Render::Model m_tableChairSet;
        Render::Model m_streetLamp;
        Render::Model m_parkBench;
        Render::Model m_billboard;
        Render::Model m_telescope;
        Render::Model m_chessSet;
        Render::Model m_tree;

        std::array<glm::vec3, 4> m_chessTablePositions {
            glm::vec3(3.5f, 0.01f, -3.5f),
            glm::vec3(3.5f, 0.01f, 3.5f),
            glm::vec3(-3.5f, 0.01f, 3.5f),
            glm::vec3(-3.5f, 0.01f, -3.5f)
        };
        std::array<glm::vec3, 4> m_chessSetPositions {
            glm::vec3(3.43f, 0.663f, -3.43f),
            glm::vec3(3.43f, 0.663f, 3.57f),
            glm::vec3(-3.57f, 0.663f, 3.57f),
            glm::vec3(-3.57f, 0.663f, -3.43f)
        };

        std::array<glm::vec3, 6> m_streetLampPositions {
            glm::vec3(-1.7f, 0.0f, -8.0f),
            glm::vec3(1.7f, 0.0f, -11.0f),
            glm::vec3(4.5f, 0.0f, 4.5f),
            glm::vec3(4.5f, 0.0f, -4.5f),
            glm::vec3(-4.5f, 0.0f, 4.5f),
            glm::vec3(-4.5f, 0.0f, -4.5f)
        };

        void m_UpdateShader(const std::shared_ptr<Render::Shader>& shader);

        void m_UpdateFlashlight(const std::shared_ptr<Render::Shader> &shader);
        void m_UpdateFountain(const std::shared_ptr<Render::Shader> &shader);
        void m_UpdateTableChairSet(const std::shared_ptr<Render::Shader> &shader);
        void m_UpdateStreetLamp(const std::shared_ptr<Render::Shader> &shader);
        void m_UpdateParkBench(const std::shared_ptr<Render::Shader> &shader);
        void m_UpdateBillboard(const std::shared_ptr<Render::Shader> &shader);
        void m_UpdateTelescope(const std::shared_ptr<Render::Shader> &shader);
        void m_UpdateChessSet(const std::shared_ptr<Render::Shader> &shader);

        void m_SetupInstancing();
        void m_UpdateInstancedTrees();
    };
}
