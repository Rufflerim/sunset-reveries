//
// Created by gaetz on 06/11/2022.
//

#ifndef SUNSET_REVERIES_SCENE_GAME_HPP
#define SUNSET_REVERIES_SCENE_GAME_HPP

#include "IScene.hpp"
#include "raylib.h"
#include "Game.hpp"
#include "../gecs/Types.hpp"

enum class TimeStatus {
    Normal,
    Rewinding,
    Pause,
    Forward
};

class SceneGame : public IScene {
public:
    SceneGame(Game& game);
    void Load() override;
    void Update(f32 dt) override;
    void Draw() override;
    void Unload() override;

private:
    gecs::Id testEntityId;

    Game& game;
    Texture2D backgroundTexture {};

    u64 CreateRandomBouncingEntity();
};


#endif //SUNSET_REVERIES_SCENE_GAME_HPP
