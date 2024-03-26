#pragma once

#include <array>
#include <vector>

#include "InputController.h"

namespace Controllers {
    enum class EventType {
        None = 0,
        MouseMoved,
        Keyboard,
        Scroll,
        Count
    };

    struct EventMouseMoved {
        double xoffset;
        double yoffset;
    };

    struct EventKeyboard {
        int key;
        Controllers::KeyState keyState;
    };

    struct EventScroll {
        double yoffset;
    };

    struct Event {
        EventType eventType;
        union {
            EventMouseMoved mouseMoved;
            EventKeyboard keyboard;
            EventScroll scroll;
        };
    };

    class Observer {
    public:
        virtual void Notify(Event event) = 0;
        virtual ~Observer() = default;
    };

    class EventController {
    public:
        EventController(const EventController&) = delete;
        EventController(EventController&&) = delete;
        EventController& operator=(const EventController&) = delete;
        EventController& operator=(EventController&&) = delete;

        static EventController& GetInstance() {
            static EventController instance;
            return instance;
        }

        void PushEvent(Event event);
        void SubscribeToEvent(EventType eventType, Observer *observer);
        void UnsubscribeFromEvent(EventType eventType, Observer *observer);
        void UnsubscribeFromAllEvents(Observer *observer);
    private:
        EventController() = default;

        std::array<std::vector<Observer*>, static_cast<unsigned long>(Controllers::EventType::Count)> m_observers;
    };

    const char *ToString(EventType event);
}
