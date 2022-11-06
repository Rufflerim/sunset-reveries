//
// Created by gaetz on 06/11/2022.
//

#include "SceneGame.h"
#include "../gaemi/Renderer.h"
#include "../gaemi/AssetsManager.h"


void SceneGame::Load() {
    AssetsManager::LoadTexture("bg_sunset", "assets/bg_sunset.png");
    texture = AssetsManager::GetTexture("bg_sunset");
}

void SceneGame::Update(float dt) {

}

void SceneGame::Draw() {
    render::DrawTexture(texture, 0, 120, WHITE);
}

void SceneGame::Unload() {

}