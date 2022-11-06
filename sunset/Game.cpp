//
// Created by gaetz on 05/11/2022.
//

#include "Game.h"
#include "../gaemi/Renderer.h"
#include "SceneGame.h"
#include <ranges>

void Game::Load() {
    AddScene(std::make_unique<SceneGame>());
}

void Game::Update(float dt) {
    for (auto&& scene : std::ranges::reverse_view(sceneStack)) {
        scene->Update(dt);
        if (scene->getLocking()) break;
    }
}

void Game::Draw(const Renderer& renderer) {
    for (auto&& scene : std::ranges::reverse_view(sceneStack)) {
        scene->Draw(renderer);
        if (!scene->getTransparent()) break;
    }
}

void Game::AddScene(unique_ptr<IScene> newScene) {
    sceneStack.push_back(std::move(newScene));
    sceneStack.back()->Load();
}

void Game::RemoveCurrentScene() {
    if (sceneStack.empty()) return;
    sceneStack.back()->Unload();
    sceneStack.pop_back();
}

void Game::SwitchScene(unique_ptr<IScene> newScene) {
    if (!sceneStack.empty()) {
        RemoveCurrentScene();
    }
    AddScene(std::move(newScene));
}

void Game::Unload() {
    for (auto&& scene : std::ranges::reverse_view(sceneStack)) {
        scene->Unload();
    }
    sceneStack.clear();
}


