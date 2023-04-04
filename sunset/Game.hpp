//
// Created by gaetz on 05/11/2022.
//

#ifndef SUNSET_REVERIES_GAME_HPP
#define SUNSET_REVERIES_GAME_HPP

#include "../gaemi/IGame.hpp"
#include "IScene.hpp"
#include "ECSManager.hpp"
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
    void Forward(u64 frameSpeed);
    void Resume(bool doCreateClone);

    void PushPlayerChange(PlayerChange playerChange);
    void ConsumePlayerChanges();

private:
    vector<unique_ptr<IScene>> sceneStack {};
    shared_ptr<ECSManager> ecs;
    unique_ptr<WorldStateManager> worldStateManager;

    vector<PlayerChange> playerChanges;
};


#endif //SUNSET_REVERIES_GAME_HPP
