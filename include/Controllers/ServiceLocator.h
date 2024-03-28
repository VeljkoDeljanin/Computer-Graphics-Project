#pragma once

#include "Controllers/InputController.h"
#include "Controllers/EventController.h"
#include "Controllers/EntityController.h"

namespace Controllers {
    class ServiceLocator {
    public:
        ServiceLocator(const ServiceLocator&) = delete;
        ServiceLocator(ServiceLocator&&) = delete;
        ServiceLocator& operator=(const ServiceLocator&) = delete;
        ServiceLocator& operator=(ServiceLocator&&) = delete;

        static ServiceLocator& GetInstance() {
            static ServiceLocator instance;
            return instance;
        }

        InputController& GetInputController() { return m_inputController; }
        EventController& GetEventController() { return m_eventController; }
        EntityController& GetEntityController() { return m_entityController; }
    private:
        ServiceLocator() = default;

        InputController& m_inputController = InputController::GetInstance();
        EventController& m_eventController = EventController::GetInstance();
        EntityController& m_entityController = EntityController::GetInstance();
    };
}
