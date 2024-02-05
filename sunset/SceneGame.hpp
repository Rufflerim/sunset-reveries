//
// Created by gaetz on 06/11/2022.
//

#ifndef SUNSET_REVERIES_SCENE_GAME_HPP
#define SUNSET_REVERIES_SCENE_GAME_HPP

#include "IScene.hpp"
#include "raylib.h"
#include "WorldStateManager.hpp"
#include "Game.hpp"
#include "EntityRingBuffer.hpp"
#include "../gecs/Types.hpp"
#include "Vec2.hpp"

using gmath::Vec2;

enum class TimeStatus {
    Normal,
    Rewinding,
    Pause,
    Forward
};

class SceneGame : public IScene {
public:
    SceneGame(//sptr<ECSManager> ecsRef,
              Game& game);
    void Load() override;
    void Update(f32 dt) override;
    void Draw() override;
    void Unload() override;

private:
    gecs::Id testEntityId;

    Game& game;
    //sptr<ECSManager> ecs;
    Texture2D backgroundTexture {};
    u64 playerId { 0 };
    //Weapon* playerWeapon { nullptr };
    //uptr<EntityRingBuffer> projectiles { nullptr };

    float jumpPressTime { 0 };
    Vec2 moveAcceleration {0.0f, 0.0f };

    u32 currentFrame { 0 };
    u32 maxCurrentFrame { 0 };
    TimeStatus timeStatus { TimeStatus::Normal };

    u8 textAlpha { 255 };
    bool textBlinkUp { false };

    /*void UpdateNormal(f32 dt);
    void UpdateRewind(f32 dt);
    void UpdatePause(f32 dt);
    void UpdateForward(f32 dt);
    */
    void DrawInterface();

    /*u64 CreateRandomBouncingEntity();*/

    const f32 PLAYER_JUMP_MAX_PRESS_TIME;
    const f32 PLAYER_HORIZONTAL_ACCELERATION;
    const f32 PLAYER_JUMP_ACCELERATION;

    const u32 PHYSICS_FRAME_REWIND_SPEED;
    const u32 PHYSICS_FRAME_FORWARD_SPEED;
};


#endif //SUNSET_REVERIES_SCENE_GAME_HPP
