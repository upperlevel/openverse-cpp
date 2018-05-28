#pragma once

#include "doctest.h"
#include "../include/event.h"

struct DummyEvent : public Event {
    int number;
};

void on_dummy_call(DummyEvent *event) {
    event->number -= 1;
}

// ---------------------------------------------------------------- Test 1

struct AnotherDummyEvent : public Event {
    long number;
};

struct SomeDummyEvent : public Event {
    char letter;
};

void on_another_dummy_call(AnotherDummyEvent *event) {
    event->number += 1;
}

void on_some_dummy_call(SomeDummyEvent *event) {
    event->letter = 'L';
}

TEST_CASE("multiple events and multiple listeners") {
    EventManager event_manager;
    event_manager.subscribe<DummyEvent>(&on_dummy_call);
    event_manager.subscribe<AnotherDummyEvent>(&on_another_dummy_call);
    event_manager.subscribe<SomeDummyEvent>(&on_some_dummy_call);

    DummyEvent dummy_event;
    dummy_event.number = 20;

    AnotherDummyEvent another_dummy_event;
    another_dummy_event.number = 10;

    SomeDummyEvent some_dummy_event;
    some_dummy_event.letter = 'A';

    event_manager.call(&dummy_event);
    event_manager.call(&another_dummy_event);
    event_manager.call(&some_dummy_event);

    CHECK(dummy_event.number == 19);
    CHECK(another_dummy_event.number == 11);
    CHECK(some_dummy_event.letter == 'L');
}

// ---------------------------------------------------------------- Test 2

TEST_CASE("call before and after listener subscription") {
    EventManager manager;

    DummyEvent event;
    event.number = 21;

    // before
    manager.call(&event);
    CHECK(event.number == 21);

    // after
    manager.subscribe<DummyEvent>(&on_dummy_call);
    manager.call(&event);
    CHECK(event.number == 20);
}

// ---------------------------------------------------------------- Test 3

void on_first_call(DummyEvent *event) {
    event->number -= 1;
    CHECK(event->number == 19);
};

void on_second_call(DummyEvent *event) {
    event->number -= 2;
    CHECK(event->number == 17);
};

void on_third_call(DummyEvent *event) {
    event->number -= 3;
    CHECK(event->number == 14);
};

TEST_CASE("multiple listeners for single event") {
    DummyEvent event;
    event.number = 20;

    EventManager manager;
    manager.subscribe<DummyEvent>(&on_first_call);
    manager.subscribe<DummyEvent>(&on_second_call);
    manager.subscribe<DummyEvent>(&on_third_call);

    manager.call(&event);
}

// ---------------------------------------------------------------- Test 4

class Listener {
public:
    void on_something(DummyEvent *event) {
        event->number = 123;
    }
};

TEST_CASE("method as listener") {
    DummyEvent event;
    event.number = 1;

    Listener listener;
    EventManager manager;
    manager.subscribe<DummyEvent>([&](auto e) { listener.on_something(e); });

    manager.call(&event);
    CHECK(event.number == 123);
}
