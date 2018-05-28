#include "scene.h"

#include <GL/gl.h>

// ---------------------------------------------------------------- ResourceScene

ResourceScene::ResourceScene(Stage &parent) {
    this->parent = &parent;
}

void ResourceScene::on_enable(Scene *prev) {
    // todo: load resources
    parent->set_scene(new GameScene(*parent));
}

void ResourceScene::on_disable(Scene *next) {
}

void ResourceScene::on_tick() {
}

void ResourceScene::on_render() {
}

// ---------------------------------------------------------------- GameScene

GameScene::GameScene(Stage &parent) {
    this->parent = &parent;
}

void GameScene::on_enable(Scene *prev) {
    parent->set_scene(new ReceiveWorldScene(*this));
}

void GameScene::on_disable(Scene *next) {
    parent->get_scene()->on_disable(nullptr);
}

void GameScene::on_tick() {
    parent->get_scene()->on_tick();
}

void GameScene::on_render() {
    parent->get_scene()->on_render();
}

// ---------------------------------------------------------------- ReceiveWorldScene

ReceiveWorldScene::ReceiveWorldScene(GameScene &parent) {
    this->parent = &parent;
}

void ReceiveWorldScene::on_enable(Scene *prev) {
}

void ReceiveWorldScene::on_disable(Scene *next) {
}

void ReceiveWorldScene::on_tick() {
}

void ReceiveWorldScene::on_render() {
    glClearColor(1.0f, 0.0f, 0.0f, 0.0f); // temp background
}

// ---------------------------------------------------------------- PlayWorldScene

PlayWorldScene::PlayWorldScene(GameScene &parent) {
    this->parent = &parent;
}

void PlayWorldScene::on_enable(Scene *prev) {
}

void PlayWorldScene::on_disable(Scene *next) {
}

void PlayWorldScene::on_tick() {
}

void PlayWorldScene::on_render() {
}
