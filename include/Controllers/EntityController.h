#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

#include "Shader.h"
#include "Entities/Entity.h"

namespace Controllers {
    class EntityController {
    public:
        EntityController(const EntityController&) = delete;
        EntityController(EntityController&&) = delete;
        EntityController& operator=(const EntityController&) = delete;
        EntityController& operator=(EntityController&&) = delete;

        static EntityController& GetInstance() {
            static EntityController instance;
            return instance;
        }

        void Init();
        void Update();
        void Clear();
    private:
        EntityController() = default;

        std::vector<std::unique_ptr<Entities::Entity>> m_entities;
        std::unordered_map<std::string, std::shared_ptr<Render::Shader>> m_shaders;

        void m_InitEntities();
        void m_InitShaders();
    };
}
