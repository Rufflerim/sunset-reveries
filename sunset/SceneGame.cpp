//
// Created by gaetz on 06/11/2022.
//

#include "SceneGame.h"

#include <utility>
#include "../gaemi/Renderer.h"
#include "../gaemi/AssetsManager.h"

SceneGame::SceneGame(shared_ptr<ECSManager> ecsRef,
                     Game& game)
: ecs {std::move( ecsRef )}, game { game }
{

}

void SceneGame::Load() {
    AssetsManager::LoadTexture("bg_sunset", "assets/bg_sunset.png", ToSceneId(SceneName::SceneGame));
    texture = AssetsManager::GetTexture("bg_sunset");
    AssetsManager::LoadTexture("player", "assets/player.png", ToSceneId(SceneName::SceneGame));

    u32 playerId = ecs->CreateEntity();

    const f32 playerX { 400 };
    const f32 playerY { 400 };
    ecs->CreateTransform2DComponent(playerId);
    ecs->GetComponent<Transform2D>(playerId).pos = { playerX, playerY };

    ecs->CreateSpriteComponent(playerId, "player");

    const auto& playerTexture = AssetsManager::GetTexture("player");
    ecs->CreateRigidbody2DComponent(playerId, { playerX, playerY }, { 0, 0, static_cast<float>(playerTexture.width), static_cast<float>(playerTexture.height)});
    ecs->GetComponent<Rigidbody2D>(playerId).velocity = { 500.0f, 500.0f };

    for (i32 i = 0; i < 1000; ++i) {
        CreateRandomBouncingEntity();
    }
}

void SceneGame::Update(f32 dt) {
    if (IsKeyDown(KEY_LEFT)) {
        game.Rewind(2);
    }

    if (IsKeyReleased(KEY_LEFT)) {
        game.Resume();
    }
}

void SceneGame::Draw() {
    render::DrawTexture(texture, 0, 120, WHITE);
}

void SceneGame::Unload() {
    AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}

u32 SceneGame::CreateRandomBouncingEntity() {
    u32 newId = ecs->CreateEntity();

    const f32 x = static_cast<f32>(GetRandomValue(100, 1100));
    const f32 y = static_cast<f32>(GetRandomValue(150, 550));

    ecs->CreateTransform2DComponent(newId);
    ecs->GetComponent<Transform2D>(newId).pos = { x, y };

    ecs->CreateSpriteComponent(newId, "player");

    const auto& playerTexture = AssetsManager::GetTexture("player");
    ecs->CreateRigidbody2DComponent(newId, { x, y }, { 0, 0, static_cast<float>(playerTexture.width), static_cast<float>(playerTexture.height)});
    ecs->GetComponent<Rigidbody2D>(newId).velocity.x = static_cast<float>(GetRandomValue(0, 600));
    ecs->GetComponent<Rigidbody2D>(newId).velocity.y = static_cast<float>(GetRandomValue(0, 600));

    return newId;
}



