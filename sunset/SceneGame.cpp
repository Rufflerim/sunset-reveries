//
// Created by gaetz on 06/11/2022.
//

#include "SceneGame.hpp"
#include <iomanip>
#include "ImRenderer.h"
#include "../gecs/World.hpp"
#include "../gecs/Entity.hpp"
#include "AssetsManager.hpp"
#include "Renderer.hpp"
#include "Query.hpp"

using gecs::Position;
using gecs::Velocity;
using gecs::Sprite;
using gecs::Query;

SceneGame::SceneGame(//sptr<ECSManager> ecsRef,
                     Game& game)
: /*ecs {std::move( ecsRef )},*/ game { game },
  PLAYER_JUMP_MAX_PRESS_TIME { AssetsManager::GetData("PLAYER_JUMP_MAX_PRESS_TIME") },
  PLAYER_HORIZONTAL_ACCELERATION { AssetsManager::GetData("PLAYER_HORIZONTAL_ACCELERATION") },
  PLAYER_JUMP_ACCELERATION { AssetsManager::GetData("PLAYER_JUMP_ACCELERATION") },
  PHYSICS_FRAME_REWIND_SPEED { static_cast<u32>(AssetsManager::GetData("PHYSICS_FRAME_REWIND_SPEED")) },
  PHYSICS_FRAME_FORWARD_SPEED { static_cast<u32>(AssetsManager::GetData("PHYSICS_FRAME_FORWARD_SPEED")) }
{

}

void SceneGame::Load() {
    AssetsManager::LoadTexture("bg_sunset", "assets/images/bg_sunset.png", ToSceneId(SceneName::SceneGame));
    AssetsManager::LoadTexture("player", "assets/images/player.png", ToSceneId(SceneName::SceneGame));
    AssetsManager::LoadTexture("ghost", "assets/images/ghost.png", ToSceneId(SceneName::SceneGame));
    AssetsManager::LoadTexture("projectile", "assets/images/projectile.png", ToSceneId(SceneName::SceneGame));

    backgroundTexture = AssetsManager::GetTexture("bg_sunset");

    gecs::World& world = gecs::World::Instance();
    world.Init();

    vector<gecs::Id> entities;
    for (u32 i = 0; i < 50; ++i) {
        auto testEntityId = world.CreateEntity();
        gecs::Entity entity = world.GetEntity(testEntityId);
        Position pos {static_cast<f32>(0 + i * 10), static_cast<f32>(0 + i * 10)};
        world.AddComponent<gecs::Position>(testEntityId, pos);
        Velocity vel {static_cast<f32>(100), 0};
        world.AddComponent<gecs::Velocity>(testEntityId, vel);
        gecs::Sprite sprite { AssetsManager::GetTexture("ghost") };
        world.AddComponent<gecs::Sprite>(testEntityId, sprite);
        entities.push_back(testEntityId);
    }

    world.DestroyEntity(entities[20]);

    world.LogWorld();






    /*

    // Player
    playerId = ecs->CreateEntity();
    const f32 playerX { 400 };
    const f32 playerY { 200 };
    auto& playerTransform = ecs->CreateTransform2DComponent(playerId);
    playerTransform.pos = Vec2 { playerX, playerY };

    ecs->CreateSpriteComponent(playerId, "player");
    const auto& playerTexture = AssetsManager::GetTexture("player");

    ecs->CreateRigidbody2DComponent(playerId, Vec2 { playerX, playerY },
                                    { 0, 0, static_cast<float>(playerTexture.width), static_cast<float>(playerTexture.height)},
                                    true, false);
    ecs->CreateBodyRaycast2DComponent(playerId, ecs, 4, 5,
                                      100.0f, 100.0f, 5.0f);


    // Projectiles and weapons
    projectiles = make_unique<EntityRingBuffer>(100, ecs);
    playerWeapon = &ecs->CreateWeaponComponent(playerId, projectiles.get());

    // Level
    auto floorId = ecs->CreateEntity();
    auto& floorTransform = ecs->CreateTransform2DComponent(floorId);
    floorTransform.pos = Vec2 { 0, 600 };
    ecs->CreateRigidbody2DComponent(floorId, Vec2 { 0, 600 },
                                   { 0, 0, 1280.0f, 120.0f},
                                   false, false);

     */
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

    auto q = Query<Position, Velocity>();
    q.Update([dt](Position& pos, Velocity& vel) {
        const Vec2 newPos { pos.x + vel.x * dt, pos.y + vel.y * dt };
        pos.Set(newPos);
    });

    q.Apply();


    /*

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

    */
}

/*
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
        moveAcceleration.y += (1.0f - gmath::EaseOutExpo(t)) * PLAYER_JUMP_ACCELERATION * dt;
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
*/

void SceneGame::Draw() {
    render::DrawTexture(backgroundTexture, 0, 120, WHITE);

    // Query all entities with a position and a sprite
    auto posSprites = gecs::Query<Position, Sprite>();
    posSprites.Read([](const Position& pos, const Sprite& spr) {
        render::DrawTexture(spr.texture, pos.x, pos.y, WHITE);
    });

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
            render::DrawDefaultText("[LEFT] Rewind", Vec2 { 700, 610 }, 20, GRAY);
            break;
        case TimeStatus::Rewinding:
            render::im::GuiSlider( Rect { 700, 690, 500, 20 }, "Time", p.str() + "%", sliderValue, 0, 100);
            render::DrawDefaultText("[LEFT] Rewind", Vec2 { 700, 610 }, 20, blinkTextColor);
            render::DrawDefaultText("[release] Pause", Vec2 { 700, 650 }, 20, GRAY);
            break;
        case TimeStatus::Pause:
            render::im::GuiSlider( Rect { 700, 690, 500, 20 }, "Time", p.str() + "%", sliderValue, 0, 100);
            render::DrawDefaultText("[LEFT] Rewind", Vec2 { 700, 610 }, 20, GRAY);
            render::DrawDefaultText("[RIGHT] Forward", Vec2 { 1000, 610 }, 20, GRAY);
            render::DrawDefaultText("[DOWN] Resume", Vec2 { 700, 650 }, 20, GRAY);
            render::DrawDefaultText("[UP] Clone", Vec2 { 1000, 650 }, 20, blinkTextColor);
            break;
        case TimeStatus::Forward:
            render::im::GuiSlider( Rect { 700, 690, 500, 20 }, "Time", p.str() + "%", sliderValue, 0, 100);
            render::DrawDefaultText("[RIGHT] Forward", Vec2 { 700, 610 }, 20, blinkTextColor);
            render::DrawDefaultText("[release] Pause", Vec2 { 700, 650 }, 20, GRAY);
            break;
    }
}

void SceneGame::Unload() {
    AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}




