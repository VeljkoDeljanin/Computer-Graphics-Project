#include "Controllers/EventController.h"

#include <algorithm>

void Controllers::EventController::PushEvent(Controllers::Event event) {
    auto& eventObservers = m_observers[static_cast<unsigned long>(event.eventType)];
    for (auto& observer : eventObservers)
        observer->Notify(event);
}

void Controllers::EventController::SubscribeToEvent(Controllers::EventType eventType, Controllers::Observer *observer) {
    auto& eventObservers = m_observers[static_cast<unsigned long>(eventType)];
    if (std::find(eventObservers.begin(), eventObservers.end(), observer) == eventObservers.end())
        eventObservers.push_back(observer);
}

void Controllers::EventController::UnsubscribeFromEvent(Controllers::EventType eventType, Controllers::Observer *observer) {
    auto& eventObservers = m_observers[static_cast<unsigned long>(eventType)];
    eventObservers.erase(
            std::remove(eventObservers.begin(), eventObservers.end(), observer),
            eventObservers.end()
    );
}

void Controllers::EventController::UnsubscribeFromAllEvents(Controllers::Observer *observer) {
    for (unsigned long event = 0; event < static_cast<unsigned long>(Controllers::EventType::Count); event++) {
        UnsubscribeFromEvent(static_cast<EventType>(event), observer);
    }
}

const char *Controllers::ToString(EventType event) {
    switch (event) {
        case EventType::MouseMoved: return "Event type: MouseMoved";
        case EventType::Keyboard: return "Event type: Keyboard";
        case EventType::Scroll: return "Event type: Scroll";
        default: return "Event: default";
    }
}
