//
// Created by gaetz on 06/11/2022.
//

#include "SceneGame.hpp"
#include <utility>
#include <iomanip>
#include "ImRenderer.h"

SceneGame::SceneGame(shared_ptr<ECSManager> ecsRef,
                     Game& game)
: ecs {std::move( ecsRef )}, game { game },
  PLAYER_JUMP_MAX_PRESS_TIME { AssetsManager::GetData("PLAYER_JUMP_MAX_PRESS_TIME") },
  PLAYER_HORIZONTAL_ACCELERATION { AssetsManager::GetData("PLAYER_HORIZONTAL_ACCELERATION") },
  PLAYER_JUMP_ACCELERATION { AssetsManager::GetData("PLAYER_JUMP_ACCELERATION") },
  PHYSICS_FRAME_REWIND_SPEED { static_cast<u32>(AssetsManager::GetData("PHYSICS_FRAME_REWIND_SPEED")) },
  PHYSICS_FRAME_FORWARD_SPEED { static_cast<u32>(AssetsManager::GetData("PHYSICS_FRAME_FORWARD_SPEED")) }
{

}

void SceneGame::Load() {
    AssetsManager::LoadTexture("bg_sunset", "assets/images/bg_sunset.png", ToSceneId(SceneName::SceneGame));
    backgroundTexture = AssetsManager::GetTexture("bg_sunset");
    AssetsManager::LoadTexture("player", "assets/images/player.png", ToSceneId(SceneName::SceneGame));
    AssetsManager::LoadTexture("ghost", "assets/images/ghost.png", ToSceneId(SceneName::SceneGame));
    AssetsManager::LoadTexture("projectile", "assets/images/projectile.png", ToSceneId(SceneName::SceneGame));

    // Player
    playerId = ecs->CreateEntity();
    const f32 playerX { 400 };
    const f32 playerY { 200 };
    auto& playerTransform = ecs->CreateTransform2DComponent(playerId);
    playerTransform.pos = { playerX, playerY };

    ecs->CreateSpriteComponent(playerId, "player");
    const auto& playerTexture = AssetsManager::GetTexture("player");

    ecs->CreateRigidbody2DComponent(playerId, { playerX, playerY },
                                    { 0, 0, static_cast<float>(playerTexture.width), static_cast<float>(playerTexture.height)},
                                    true, false);
    ecs->CreateBodyRaycast2DComponent(playerId, ecs, 4, 5,
                                      100.0f, 100.0f, 5.0f);
    playerWeapon = &ecs->CreateWeaponComponent(playerId, ecs);



    auto floorId = ecs->CreateEntity();
    auto& floorTransform = ecs->CreateTransform2DComponent(floorId);
    floorTransform.pos = { 0, 600 };
    ecs->CreateRigidbody2DComponent(floorId, { 0, 600 },
                                   { 0, 0, 1280.0f, 120.0f},
                                   false, false);

    /*
    auto platform1Id = ecs->CreateEntity();
    ecs->CreateTransform2DComponent(platform1Id);
    ecs->GetComponent<Transform2D>(platform1Id).pos = { 80, 500 };
    ecs->CreateRigidbody2DComponent(platform1Id, { 80, 500 },
                                    { 0, 0, 200.0f, 50.0f},
                                    false, false);

    auto platform2Id = ecs->CreateEntity();
    ecs->CreateTransform2DComponent(platform2Id);
    ecs->GetComponent<Transform2D>(platform2Id).pos = { 500, 400 };
    ecs->CreateRigidbody2DComponent(platform2Id, { 500, 400 },
                                    { 0, 0, 300.0f, 50.0f},
                                    false, false);

    auto platform3Id = ecs->CreateEntity();
    ecs->CreateTransform2DComponent(platform3Id);
    ecs->GetComponent<Transform2D>(platform3Id).pos = { 800, 500 };
    ecs->CreateRigidbody2DComponent(platform3Id, { 800, 500 },
                                    { 0, 0, 200.0f, 50.0f},
                                    false, false);
    */

}

void SceneGame::Update(f32 dt) {
    switch (timeStatus) {
        case TimeStatus::Normal:
            UpdateNormal(dt);
            break;
        case TimeStatus::Rewinding:
            UpdateRewind(dt);
            break;
        case TimeStatus::Pause:
            UpdatePause(dt);
            break;
        case TimeStatus::Forward:
            UpdateForward(dt);
            break;
    }
}

void SceneGame::UpdateNormal(f32 dt) {
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

    // Shoot
    if (IsKeyDown(KEY_LEFT_CONTROL)) {
        playerWeapon->Shoot();
    }

    // Time control
    if (IsKeyDown(KEY_LEFT)) {
        maxCurrentFrame = currentFrame;
        timeStatus = TimeStatus::Rewinding;
        // Will have to check if we should return to skip a frame
    }

    ++currentFrame;
    ecs->SetCurrentFrame(currentFrame);
}

void SceneGame::UpdateRewind(f32 dt) {
    if (IsKeyDown(KEY_LEFT)) {
        game.Rewind(PHYSICS_FRAME_REWIND_SPEED);
        if (currentFrame > PHYSICS_FRAME_REWIND_SPEED) {
            currentFrame -= PHYSICS_FRAME_REWIND_SPEED;
        } else {
            currentFrame = PHYSICS_FRAME_REWIND_SPEED;
        }
    }
    else {
        timeStatus = TimeStatus::Pause;
    }
}

void SceneGame::UpdatePause(f32 dt) {
    // Clone
    if (IsKeyPressed(KEY_UP)) {
        game.Resume(true);
        timeStatus = TimeStatus::Normal;
    }
    // Get back to normal time
    else if (IsKeyPressed(KEY_DOWN)) {
        game.Resume(false);
        timeStatus = TimeStatus::Normal;
    }
    else if (IsKeyPressed(KEY_LEFT)) {
        timeStatus = TimeStatus::Rewinding;
    }
    else if (IsKeyPressed(KEY_RIGHT)) {
        timeStatus = TimeStatus::Forward;
    }
}

void SceneGame::UpdateForward(f32 dt) {
    if (IsKeyDown(KEY_RIGHT)) {
        game.Forward(PHYSICS_FRAME_FORWARD_SPEED);
        currentFrame += PHYSICS_FRAME_FORWARD_SPEED;
        if (currentFrame > maxCurrentFrame) {
            currentFrame = maxCurrentFrame;
        }
    } else {
        timeStatus = TimeStatus::Pause;
    }
}

void SceneGame::Draw() {
    render::DrawTexture(backgroundTexture, 0, 120, WHITE);
    DrawInterface();
}

void SceneGame::DrawInterface() {
    const u8 blinkSpeed { 10 };
    textAlpha = textBlinkUp ? textAlpha + blinkSpeed : textAlpha - blinkSpeed;
    if (textAlpha < blinkSpeed) {
        textAlpha = 0;
        textBlinkUp = !textBlinkUp;
    } else if (textAlpha > 255 - blinkSpeed) {
        textAlpha = 255;
        textBlinkUp = !textBlinkUp;
    }
    const Color blinkTextColor { 130, 130, 130, textAlpha };

    f32 sliderValue = static_cast<f32>(currentFrame) / static_cast<f32>(maxCurrentFrame) * 100.0f;
    std::stringstream p;
    p << std::fixed <<  std::setprecision(1) << sliderValue;
    switch (timeStatus) {
        case TimeStatus::Normal:
            render::DrawDefaultText("[LEFT] Rewind", Vector2 { 700, 610 }, 20, GRAY);
            break;
        case TimeStatus::Rewinding:
            render::im::GuiSlider( Rectangle { 700, 690, 500, 20 }, "Time", p.str() + "%", sliderValue, 0, 100);
            render::DrawDefaultText("[LEFT] Rewind", Vector2 { 700, 610 }, 20, blinkTextColor);
            render::DrawDefaultText("[release] Pause", Vector2 { 700, 650 }, 20, GRAY);
            break;
        case TimeStatus::Pause:
            render::im::GuiSlider( Rectangle { 700, 690, 500, 20 }, "Time", p.str() + "%", sliderValue, 0, 100);
            render::DrawDefaultText("[LEFT] Rewind", Vector2 { 700, 610 }, 20, GRAY);
            render::DrawDefaultText("[RIGHT] Forward", Vector2 { 1000, 610 }, 20, GRAY);
            render::DrawDefaultText("[DOWN] Resume", Vector2 { 700, 650 }, 20, GRAY);
            render::DrawDefaultText("[UP] Clone", Vector2 { 1000, 650 }, 20, blinkTextColor);
            break;
        case TimeStatus::Forward:
            render::im::GuiSlider( Rectangle { 700, 690, 500, 20 }, "Time", p.str() + "%", sliderValue, 0, 100);
            render::DrawDefaultText("[RIGHT] Forward", Vector2 { 700, 610 }, 20, blinkTextColor);
            render::DrawDefaultText("[release] Pause", Vector2 { 700, 650 }, 20, GRAY);
            break;
    }
}

void SceneGame::Unload() {
    AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}

u64 SceneGame::CreateRandomBouncingEntity() {
    u64 newId = ecs->CreateEntity();

    const f32 x = static_cast<f32>(GetRandomValue(100, 1100));
    const f32 y = static_cast<f32>(GetRandomValue(150, 550));

    auto& transform = ecs->CreateTransform2DComponent(newId);
    transform.pos = { x, y };

    ecs->CreateSpriteComponent(newId, "player");
    const auto& playerTexture = AssetsManager::GetTexture("player");

    auto& body = ecs->CreateRigidbody2DComponent(newId, { x, y },
                                    { 0, 0, static_cast<float>(playerTexture.width), static_cast<float>(playerTexture.height)},
                                    true, false);
    body.velocity.x = static_cast<float>(GetRandomValue(0, 600));
    body.velocity.y = static_cast<float>(GetRandomValue(0, 600));

    return newId;
}





