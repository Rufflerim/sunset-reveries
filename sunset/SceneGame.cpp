//
// Created by gaetz on 06/11/2022.
//

#include "SceneGame.hpp"

#include <utility>

SceneGame::SceneGame(shared_ptr<ECSManager> ecsRef,
                     Game& game)
: ecs {std::move( ecsRef )}, game { game }
{

}

void SceneGame::Load() {
    AssetsManager::LoadTexture("bg_sunset", "assets/bg_sunset.png", ToSceneId(SceneName::SceneGame));
    texture = AssetsManager::GetTexture("bg_sunset");
    AssetsManager::LoadTexture("player", "assets/player.png", ToSceneId(SceneName::SceneGame));
    AssetsManager::LoadTexture("ghost", "assets/ghost.png", ToSceneId(SceneName::SceneGame));

    // Player
    playerId = ecs->CreateEntity();
    const f32 playerX { 400 };
    const f32 playerY { 200 };
    ecs->CreateTransform2DComponent(playerId);
    ecs->GetComponent<Transform2D>(playerId).pos = { playerX, playerY };

    ecs->CreateSpriteComponent(playerId, "player");
    const auto& playerTexture = AssetsManager::GetTexture("player");

    ecs->CreateRigidbody2DComponent(playerId, { playerX, playerY },
                                    { 0, 0, static_cast<float>(playerTexture.width), static_cast<float>(playerTexture.height)},
                                    true, false);

    ecs->CreateBodyRaycast2DComponent(playerId, ecs, 4, 3,
                                      100.0f, 100.0f, 5.0f);


    auto floorId = ecs->CreateEntity();
    ecs->CreateTransform2DComponent(floorId);
    ecs->GetComponent<Transform2D>(floorId).pos = { 0, 600 };
    ecs->CreateRigidbody2DComponent(floorId, { 0, 600 },
                                   { 0, 0, 1280.0f, 120.0f},
                                   false, false);

    /*
    auto platform1Id = ecs->CreateEntity();
    ecs->CreateTransform2DComponent(platform1Id);
    ecs->GetComponent<Transform2D>(platform1Id).pos = { 80, 500 };
    ecs->CreateRigidbody2DComponent(platform1Id, { 80, 500 },
                                    { 0, 0, 200.0f, 50.0f},
                                    false);

    auto platform2Id = ecs->CreateEntity();
    ecs->CreateTransform2DComponent(platform2Id);
    ecs->GetComponent<Transform2D>(platform2Id).pos = { 500, 400 };
    ecs->CreateRigidbody2DComponent(platform2Id, { 500, 400 },
                                    { 0, 0, 300.0f, 50.0f},
                                    false);

    auto platform3Id = ecs->CreateEntity();
    ecs->CreateTransform2DComponent(platform3Id);
    ecs->GetComponent<Transform2D>(platform3Id).pos = { 800, 500 };
    ecs->CreateRigidbody2DComponent(platform3Id, { 800, 500 },
                                    { 0, 0, 200.0f, 50.0f},
                                    false);
                                    */
}

void SceneGame::Update(f32 dt) {
    // Player movement
    auto& playerBody = ecs->GetComponent<Rigidbody2D>(playerId);
    if (IsKeyDown(KEY_D)) {
        moveAcceleration.x += PLAYER_HORIZONTAL_ACCELERATION * dt;
    } else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_Q)) {
        moveAcceleration.x += -PLAYER_HORIZONTAL_ACCELERATION * dt;
    }

    // Jump
    if (IsKeyDown(KEY_SPACE) && jumpPressTime < PLAYER_JUMP_MAX_PRESS_TIME) {
        f32 t = jumpPressTime / PLAYER_JUMP_MAX_PRESS_TIME;
        moveAcceleration.y += (1.0f - EaseOutExpo(t)) * PLAYER_JUMP_ACCELERATION * dt;
        jumpPressTime += dt;
    }
    if (jumpPressTime >= PLAYER_JUMP_MAX_PRESS_TIME || IsKeyReleased(KEY_SPACE)) {
        moveAcceleration.y = 0;
    }
    if (playerBody.isGrounded) {
        jumpPressTime = 0;
    }

    if (moveAcceleration.x != 0 || moveAcceleration.y != 0 ) {
        // Apply player changes
        PlayerChange playerChange {playerId, moveAcceleration };
        game.PushPlayerChange(playerChange);
        moveAcceleration.x = 0;
    }

    // Time manipulation
    if (IsKeyReleased(KEY_LEFT)) {
        game.Resume();
    }

    if (IsKeyDown(KEY_LEFT)) {
        const u32 frameSpeed = 2; // TODO Place this constant somewhere else
        game.Rewind(frameSpeed);
        if (currentFrame + frameSpeed > frameSpeed + 1) {
            currentFrame = currentFrame - frameSpeed;
        } else {
            currentFrame = 1;
        }
        return;
    }

    ++currentFrame;
    ecs->SetCurrentFrame(currentFrame);
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
    ecs->CreateRigidbody2DComponent(newId, { x, y },
                                    { 0, 0, static_cast<float>(playerTexture.width), static_cast<float>(playerTexture.height)},
                                    true, false);
    ecs->GetComponent<Rigidbody2D>(newId).velocity.x = static_cast<float>(GetRandomValue(0, 600));
    ecs->GetComponent<Rigidbody2D>(newId).velocity.y = static_cast<float>(GetRandomValue(0, 600));

    return newId;
}



