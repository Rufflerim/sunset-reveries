//
// Created by gaetz on 06/11/2022.
//

#ifndef SUNSET_REVERIES_SCENEGAME_H
#define SUNSET_REVERIES_SCENEGAME_H

#include "IScene.h"
#include "raylib.h"
#include "ECSManager.h"
#include "WorldStateManager.hpp"
#include "Game.h"

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
    Texture2D texture;
    u32 playerId;
    float jumpPressTime { 0 };
    Vector2 moveAcceleration {0.0f, 0.0f };

    u32 CreateRandomBouncingEntity();

    const f32 PLAYER_JUMP_MAX_PRESS_TIME = .085f;
    const f32 PLAYER_HORIZONTAL_ACCELERATION = 3000.0f;
    const f32 PLAYER_JUMP_ACCELERATION = -4000.0f;
};


#endif //SUNSET_REVERIES_SCENEGAME_H
