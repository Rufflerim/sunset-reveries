//
// Created by gaetz on 05/11/2022.
//

#ifndef GAEMI_IGAME_H
#define GAEMI_IGAME_H

#include "Defines.h"

class IGame {
public:
    virtual ~IGame() = default;

    virtual void Load() = 0;
    virtual void Update(f32 dt) = 0;
    virtual void Draw() = 0;
    virtual void Unload() = 0;
};

#endif //GAEMI_IGAME_H
