//
// Created by gaetz on 06/11/2022.
//

#ifndef SUNSET_REVERIES_ISCENE_H
#define SUNSET_REVERIES_ISCENE_H

#include "Defines.h"

class Renderer;

enum class SceneName {
    SceneGame = 0
};

class IScene {
public:
    virtual ~IScene() = default;

    virtual void Load() = 0;
    virtual void Update(float dt) = 0;
    virtual void Draw() = 0;
    virtual void Unload() = 0;

    bool GetLocking() const { return isLocking; }
    bool GetTransparent() const { return isTransparent; }

    i32 ToSceneId(SceneName sceneName) { return static_cast<i32>(sceneName); }

protected:
    bool isLocking { true };
    bool isTransparent { false };
};

#endif //SUNSET_REVERIES_ISCENE_H
