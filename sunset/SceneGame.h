//
// Created by gaetz on 06/11/2022.
//

#ifndef SUNSET_REVERIES_SCENEGAME_H
#define SUNSET_REVERIES_SCENEGAME_H

#include "IScene.h"
#include "raylib.h"
#include "ECSManager.h"

#include <vector>
#include <array>
#include <memory>

using std::vector;
using std::array;
using std::unique_ptr;

class SceneGame : public IScene {
public:
    void Load() override;
    void Update(f32 dt) override;
    void Draw() override;
    void Unload() override;

private:
    unique_ptr<ECSManager> ecs;
    Texture2D texture;

    u32 CreateRandomBouncingEntity();
};


#endif //SUNSET_REVERIES_SCENEGAME_H
