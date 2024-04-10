#include "Controllers/EntityController.h"

#include "Entities/Light.h"
#include "Entities/Model3D.h"
#include "Entities/Ground.h"
#include "Entities/LightBox.h"
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
    m_entities.push_back(std::make_unique<Entities::Light>(m_shaders["entity"], m_shaders["normalMapEntity"],
                                                           m_shaders["normalAndHeightMapEntity"], m_shaders["instancedEntity"]));
    m_entities.push_back(std::make_unique<Entities::Model3D>(m_shaders["entity"], m_shaders["normalMapEntity"],
                                                             m_shaders["instancedEntity"]));
    m_entities.push_back(std::make_unique<Entities::Ground>(m_shaders["entity"], m_shaders["normalMapEntity"],
                                                            m_shaders["normalAndHeightMapEntity"]));
    m_entities.push_back(std::make_unique<Entities::LightBox>(m_shaders["lightBox"]));
    m_entities.push_back(std::make_unique<Entities::Skybox>(m_shaders["skybox"]));
}

void Controllers::EntityController::m_InitShaders() {
    m_shaders["entity"]                   = std::make_shared<Render::Shader>("resources/shaders/entity.vs",
                                                                             "resources/shaders/entity.fs");
    m_shaders["normalMapEntity"]          = std::make_shared<Render::Shader>("resources/shaders/normalMapEntity.vs",
                                                                             "resources/shaders/normalMapEntity.fs");
    m_shaders["normalAndHeightMapEntity"] = std::make_shared<Render::Shader>("resources/shaders/normalMapEntity.vs",
                                                                             "resources/shaders/normalAndHeightMapEntity.fs");
    m_shaders["skybox"]                   = std::make_shared<Render::Shader>("resources/shaders/skybox.vs",
                                                                             "resources/shaders/skybox.fs");
    m_shaders["instancedEntity"]          = std::make_shared<Render::Shader>("resources/shaders/instancedEntity.vs",
                                                                             "resources/shaders/entity.fs");
    m_shaders["lightBox"]                 = std::make_shared<Render::Shader>("resources/shaders/entity.vs",
                                                                             "resources/shaders/lightBox.fs");
}
