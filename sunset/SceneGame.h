//
// Created by gaetz on 06/11/2022.
//

#ifndef SUNSET_REVERIES_SCENEGAME_H
#define SUNSET_REVERIES_SCENEGAME_H

#include "IScene.h"
#include "raylib.h"
#include "ECSManager.h"
#include "WorldStateManager.hpp"

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
              shared_ptr<WorldStateManager> worldStateManager);
    void Load() override;
    void Update(f32 dt) override;
    void Draw() override;
    void Unload() override;

private:
    shared_ptr<ECSManager> ecs;
    shared_ptr<WorldStateManager> worldStateManager;
    Texture2D texture;

    bool pause { false };

    u32 CreateRandomBouncingEntity();
};


#endif //SUNSET_REVERIES_SCENEGAME_H
