#include "Controllers/EntityController.h"

#include "Entities/Skybox.h"

void Controllers::EntityController::Init() {
    m_InitShaders();
    m_InitEntities();
}

void Controllers::EntityController::Update() {
    for (auto& entity : m_entities)
        entity.second->Update();
}

void Controllers::EntityController::Clear() {
    m_entities.clear();
    m_shaders.clear();
}

void Controllers::EntityController::m_InitEntities() {
    m_entities["skybox"] = std::make_unique<Entities::Skybox>(m_shaders["skybox"]);
}

void Controllers::EntityController::m_InitShaders() {
    m_shaders["skybox"] = std::make_shared<Render::Shader>();
    m_shaders["skybox"]->Compile("resources/shaders/skybox.vs", "resources/shaders/skybox.fs");
}
