#pragma once

#include "Controllers/InputController.h"
#include "Controllers/EventController.h"

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
    private:
        ServiceLocator() = default;

        Controllers::InputController& m_inputController = Controllers::InputController::GetInstance();
        Controllers::EventController& m_eventController = Controllers::EventController::GetInstance();
    };
}
