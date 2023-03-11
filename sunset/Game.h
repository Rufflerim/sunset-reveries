//
// Created by gaetz on 05/11/2022.
//

#ifndef SUNSET_REVERIES_GAME_H
#define SUNSET_REVERIES_GAME_H

#include "../gaemi/IGame.h"
#include "IScene.h"
#include "ECSManager.h"
#include "WorldStateManager.hpp"

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

    void Rewind(u64 frameSpeed);
    void Resume();

    void PushPlayerChange(PlayerChange playerChange);
    void ConsumePlayerChanges();

private:
    vector<unique_ptr<IScene>> sceneStack {};
    shared_ptr<ECSManager> ecs;
    unique_ptr<WorldStateManager> worldStateManager;

    vector<PlayerChange> playerChanges;
};


#endif //SUNSET_REVERIES_GAME_H
