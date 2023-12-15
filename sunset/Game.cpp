//
// Created by gaetz on 05/11/2022.
//

#include "Game.hpp"
#include "SceneGame.hpp"
#include <ranges>

void Game::Load() {

    /*
    ecs = std::make_shared<ECSManager>();
    worldStateManager = std::make_unique<WorldStateManager>(ecs);
     */
    AddScene(std::make_unique<SceneGame>(ecs, *this));
}

void Game::Update(f32 dt) {
    for (auto&& scene : std::ranges::reverse_view(sceneStack)) {
        scene->Update(dt);
        if (scene->GetLocking()) break;
    }

    /*
    if (worldStateManager->IsRecording()) ecs->Update(dt, worldChanger);
    WorldState newWorldState = worldChanger.UpdateWorld(
            ecs->GetCurrentWorldState());
    worldStateManager->StoreNewState(newWorldState);

    ecs->EndUpdate();
    worldChanger.ClearFrameChanges();
     */
}

void Game::Draw() {
    //ecs->PrepareDraw();
    for (auto&& scene : std::ranges::reverse_view(sceneStack)) {
        scene->Draw();
        if (!scene->GetTransparent()) break;
    }
    //ecs->RenderWorld();
}

void Game::AddScene(uptr<IScene> newScene) {
    sceneStack.push_back(std::move(newScene));
    sceneStack.back()->Load();
}

void Game::RemoveCurrentScene() {
    if (sceneStack.empty()) return;
    sceneStack.back()->Unload();
    sceneStack.pop_back();
}

void Game::SwitchScene(uptr<IScene> newScene) {
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

void Game::Rewind(u64 frameSpeed) {
    worldStateManager->Rewind(frameSpeed);
}

void Game::Forward(u64 frameSpeed) {
    worldStateManager->Forward(frameSpeed);
}

void Game::Resume(bool doCreateClone) {
    if (doCreateClone) {
        worldStateManager->CloneAndResume();
    } else {
        worldStateManager->Resume();
    }
}

void Game::PushPlayerChange(PlayerChange playerChange) {
    //playerChanges.emplace_back(playerChange);
    worldChanger.PushPlayerChange(playerChange);
}

