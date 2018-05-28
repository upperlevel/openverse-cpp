#pragma once

#include <memory>

// -------------------------------- Scene

class Scene {
public:
    virtual void on_enable(Scene *prev) {};
    virtual void on_disable(Scene *next) {};

    virtual void on_tick() {};
    virtual void on_render() {};
};

// -------------------------------- Stage

class Stage {
private:
    Scene *curr;

public:
    inline Scene *get_scene() const {
        return curr;
    };

    void set_scene(Scene *scene) {
        if (curr) {
            curr->on_disable(scene);
        }
        Scene *old = curr;
        curr = scene;
        if (curr) {
            curr->on_enable(old);
        }
    }
};

// -------------------------------- ResourceScene

class ResourceScene : public Scene {
private:
    Stage *parent;

public:
    ResourceScene(Stage &parent);

    void on_enable(Scene *prev);
    void on_disable(Scene *next);

    void on_tick();
    void on_render();
};

// -------------------------------- GameScene

class GameScene : public Scene {
private:
    Stage *parent;

public:
    GameScene(Stage &parent);

    void on_enable(Scene *prev);
    void on_disable(Scene *next);

    void on_tick();
    void on_render();
};

// -------------------------------- ReceiveWorldScene

class ReceiveWorldScene : public Scene {
private:
    GameScene *parent;

public:
    ReceiveWorldScene(GameScene &parent);

    void on_enable(Scene *prev);
    void on_disable(Scene *next);

    void on_tick();
    void on_render();
};

// -------------------------------- PlayWorldScene

class PlayWorldScene : public Scene {
private:
    GameScene *parent;

public:
    PlayWorldScene(GameScene &parent);

    void on_enable(Scene *prev);
    void on_disable(Scene *next);

    void on_tick();
    void on_render();
};
