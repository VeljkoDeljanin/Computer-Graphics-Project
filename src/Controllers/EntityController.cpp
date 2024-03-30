#include "Controllers/EntityController.h"

#include "Entities/Light.h"
#include "Entities/Model3D.h"
#include "Entities/Ground.h"
#include "Entities/Skybox.h"

void Controllers::EntityController::Init() {
    m_InitShaders();
    m_InitEntities();
}

void Controllers::EntityController::Update() {
    for (auto& entity : m_entities)
        entity->Update();
}

void Controllers::EntityController::Clear() {
    m_entities.clear();
    m_shaders.clear();
}

void Controllers::EntityController::m_InitEntities() {
    m_entities.push_back(std::make_unique<Entities::Light>(m_shaders["entity"]));
    m_entities.push_back(std::make_unique<Entities::Model3D>(m_shaders["entity"]));
    m_entities.push_back(std::make_unique<Entities::Ground>(m_shaders["entity"]));
    m_entities.push_back(std::make_unique<Entities::Skybox>(m_shaders["skybox"]));
}

void Controllers::EntityController::m_InitShaders() {
    m_shaders["entity"] = std::make_shared<Render::Shader>("resources/shaders/entityShader.vs", "resources/shaders/entityShader.fs");
    m_shaders["skybox"] = std::make_shared<Render::Shader>("resources/shaders/skybox.vs", "resources/shaders/skybox.fs");
}
