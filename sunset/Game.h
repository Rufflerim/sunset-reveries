//
// Created by gaetz on 05/11/2022.
//

#ifndef SUNSET_REVERIES_GAME_H
#define SUNSET_REVERIES_GAME_H

#include "../gaemi/IGame.h"

class Game : public IGame {
public:
    void Load() override;
    void Update(float dt) override;
    void Draw(const Renderer& renderer) override;
};


#endif //SUNSET_REVERIES_GAME_H
