//
// Created by gaetz on 06/11/2022.
//

#include "SceneGame.h"

#include <utility>
#include "../gaemi/Renderer.h"
#include "../gaemi/AssetsManager.h"

SceneGame::SceneGame(shared_ptr<ECSManager> ecsRef,
                     shared_ptr<WorldStateManager> worldStateManager)
: ecs {std::move( ecsRef )}, worldStateManager {std::move( worldStateManager )}
{

}

void SceneGame::Load() {
    AssetsManager::LoadTexture("bg_sunset", "assets/bg_sunset.png", ToSceneId(SceneName::SceneGame));
    texture = AssetsManager::GetTexture("bg_sunset");
    AssetsManager::LoadTexture("player", "assets/player.png", ToSceneId(SceneName::SceneGame));


    u32 playerId = ecs->CreateEntity();
    ecs->CreateTransform2DComponent(playerId);
    ecs->GetComponent<Transform2D>(playerId).pos.x = 400;
    ecs->GetComponent<Transform2D>(playerId).pos.y = 400;

    ecs->CreateSpriteComponent(playerId, "player");

    const auto& playerTexture = AssetsManager::GetTexture("player");
    ecs->CreateRigidbody2DComponent(playerId, { 0, 0, static_cast<float>(playerTexture.width), static_cast<float>(playerTexture.height)});
    ecs->GetComponent<Rigidbody2D>(playerId).velocity.x = 500.0f;
    ecs->GetComponent<Rigidbody2D>(playerId).velocity.y = 500.0f;

    for (i32 i = 0; i < 15; ++i) {
        CreateRandomBouncingEntity();
    }
}

void SceneGame::Update(f32 dt) {
    if (IsKeyDown(KEY_LEFT)) {
        pause = true;
        worldStateManager->Rewind(4);
    }

    if (IsKeyReleased(KEY_LEFT)) {
        pause = false;
        worldStateManager->Resume();
    }

    if (pause) return;

    ecs->UpdateSceneGame(dt);
}

void SceneGame::Draw() {
    render::DrawTexture(texture, 0, 120, WHITE);
    ecs->DrawSceneGame();
}

void SceneGame::Unload() {
    AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}

u32 SceneGame::CreateRandomBouncingEntity() {
    u32 newId = ecs->CreateEntity();

    ecs->CreateTransform2DComponent(newId);
    ecs->GetComponent<Transform2D>(newId).pos.x = static_cast<float>(GetRandomValue(100, 1100));
    ecs->GetComponent<Transform2D>(newId).pos.y = static_cast<float>(GetRandomValue(150, 550));

    ecs->CreateSpriteComponent(newId, "player");

    const auto& playerTexture = AssetsManager::GetTexture("player");
    ecs->CreateRigidbody2DComponent(newId, { 0, 0, static_cast<float>(playerTexture.width), static_cast<float>(playerTexture.height)});
    ecs->GetComponent<Rigidbody2D>(newId).velocity.x = static_cast<float>(GetRandomValue(0, 600));
    ecs->GetComponent<Rigidbody2D>(newId).velocity.y = static_cast<float>(GetRandomValue(0, 600));

    return newId;
}



