//
// Created by gaetz on 05/11/2022.
//

#ifndef SUNSET_REVERIES_GAME_H
#define SUNSET_REVERIES_GAME_H

#include "../gaemi/IGame.h"
#include "IScene.h"
#include "ECSManager.h"
#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;
using std::shared_ptr;

class Game : public IGame {
public:
    void Load() override;
    void Update(f32 dt) override;
    void Draw() override;
    void Unload() override;

    void AddScene(unique_ptr<IScene> newScene);
    void RemoveCurrentScene();
    void SwitchScene(unique_ptr<IScene> newScene);

private:
    vector<unique_ptr<IScene>> sceneStack {};
    shared_ptr<ECSManager> ecs;
};


#endif //SUNSET_REVERIES_GAME_H
