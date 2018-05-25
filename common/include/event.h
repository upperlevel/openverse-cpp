#pragma once

#include <unordered_map>
#include <vector>
#include <functional>
#include <typeindex>
#include <any>

struct Event {
};

class EventManager {
private:
    using Id = std::type_index;
    using Listener = std::function<void(Event *)>;
    std::unordered_map<Id, std::vector<Listener>> events;

public:
    EventManager() = default;

    template<typename E>
    void subscribe(std::function<void(E *)> action) {
        auto id = std::type_index(typeid(E));
        auto got = events.find(id);
        if (got == events.end()) {
            events[id] = std::vector<Listener>();
        }
        events[id].push_back([action](Event *event) { // NO: &action (why?)
            action(static_cast<E *>(event));
        });
    }

    template<typename E>
    int call(E *event) {
        auto id = std::type_index(typeid(E));
        auto got = events.find(id);
        if (got == events.end()) {
            return 0;
        }
        int i = 0;
        for (Listener listener : got->second) {
            listener(event);
            i++;
        }
        return i;
    }
};
