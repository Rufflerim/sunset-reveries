//
// Created by gaetz on 05/11/2022.
//

#ifndef SUNSET_REVERIES_GAME_HPP
#define SUNSET_REVERIES_GAME_HPP

#include "../gaemi/IGame.hpp"
#include "IScene.hpp"
#include "ECSManager.hpp"
#include "WorldStateManager.hpp"

class Game : public IGame {
public:
    Game() = default;
    ~Game() override = default;
    Game(Game&) = delete;
    Game& operator=(Game&) = delete;

    void Load() override;
    void Update(f32 dt) override;
    void Draw() override;
    void Unload() override;

    void AddScene(uptr<IScene> newScene);
    void RemoveCurrentScene();
    void SwitchScene(uptr<IScene> newScene);

    void Rewind(u64 frameSpeed);
    void Forward(u64 frameSpeed);
    void Resume(bool doCreateClone);

    void PushPlayerChange(PlayerChange playerChange);

private:
    vector<uptr<IScene>> sceneStack {};
    sptr<ECSManager> ecs;
    WorldChanger worldChanger;
    uptr<WorldStateManager> worldStateManager;
};


#endif //SUNSET_REVERIES_GAME_HPP
