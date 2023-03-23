//
// Created by gaetz on 05/11/2022.
//

#ifndef GAEMI_I_GAME_HPP
#define GAEMI_I_GAME_HPP

#include "Defines.hpp"

class IGame {
public:
    virtual ~IGame() = default;

    virtual void Load() = 0;
    virtual void Update(f32 dt) = 0;
    virtual void Draw() = 0;
    virtual void Unload() = 0;
};

#endif //GAEMI_I_GAME_HPP
