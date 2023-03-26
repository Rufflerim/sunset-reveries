//
// Created by gaetz on 06/11/2022.
//

#ifndef SUNSET_REVERIES_SCENE_GAME_HPP
#define SUNSET_REVERIES_SCENE_GAME_HPP

#include "IScene.hpp"
#include "raylib.h"
#include "ECSManager.hpp"
#include "WorldStateManager.hpp"
#include "Game.hpp"

#include <vector>
#include <array>
#include <memory>

using std::vector;
using std::array;
using std::unique_ptr;
using std::shared_ptr;

class SceneGame : public IScene {
public:
    SceneGame(shared_ptr<ECSManager> ecsRef,
              Game& game);
    void Load() override;
    void Update(f32 dt) override;
    void Draw() override;
    void Unload() override;

private:
    Game& game;
    shared_ptr<ECSManager> ecs;
    Texture2D texture {};
    u32 playerId { 0 };
    float jumpPressTime { 0 };
    Vector2 moveAcceleration {0.0f, 0.0f };
    u32 currentFrame { 0 };

    u32 CreateRandomBouncingEntity();

    const f32 PLAYER_JUMP_MAX_PRESS_TIME;
    const f32 PLAYER_HORIZONTAL_ACCELERATION;
    const f32 PLAYER_JUMP_ACCELERATION;

    const i32 PHYSICS_FRAME_REWIND_SPEED;
};


#endif //SUNSET_REVERIES_SCENE_GAME_HPP
