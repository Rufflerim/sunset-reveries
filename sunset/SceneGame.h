//
// Created by gaetz on 06/11/2022.
//

#ifndef SUNSET_REVERIES_SCENEGAME_H
#define SUNSET_REVERIES_SCENEGAME_H

#include "IScene.h"
#include "raylib.h"

class SceneGame : public IScene {
public:
    void Load() override;
    void Update(float dt) override;
    void Draw() override;
    void Unload() override;

private:
    Texture2D texture;
};


#endif //SUNSET_REVERIES_SCENEGAME_H
