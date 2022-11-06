//
// Created by gaetz on 05/11/2022.
//

#ifndef GAEMI_ENGINE_H
#define GAEMI_ENGINE_H

#include <string>
#include <memory>
#include "Renderer.h"
#include "Defines.h"

using std::unique_ptr;

class IGame;

class Engine {
public:
    GAPI void Start(i32 windowWidth, i32 windowHeight, const str& gameName,
                                     unique_ptr<IGame>&& gameP) noexcept;

    str gameName;
    unique_ptr<IGame> game;
    Renderer renderer {};
};


#endif //GAEMI_ENGINE_H
