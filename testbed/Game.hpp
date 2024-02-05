//
// Created by gaetz on 05/11/2022.
//

#ifndef SUNSET_REVERIES_GAME_HPP
#define SUNSET_REVERIES_GAME_HPP

#include "../gaemi/IGame.hpp"
#include "IScene.hpp"

class Game : public IGame {
public:
    void Load() override;
    void Update(f32 dt) override;
    void Draw() override;
    void Unload() override;

    void AddScene(uptr<IScene> newScene);
    void RemoveCurrentScene();
    void SwitchScene(uptr<IScene> newScene);

private:
    vector<uptr<IScene>> sceneStack {};
};


#endif //SUNSET_REVERIES_GAME_HPP
