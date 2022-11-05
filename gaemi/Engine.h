//
// Created by gaetz on 05/11/2022.
//

#ifndef GAEMI_ENGINE_H
#define GAEMI_ENGINE_H

#include <string>
#include <memory>
#include "Renderer.h"

using std::unique_ptr;

class IGame;

class Engine {
public:
    __declspec(dllexport) void Start(int windowWidth, int windowHeight, const std::string& gameName,
                                     unique_ptr<IGame> gameP);

    std::string gameName;
    unique_ptr<IGame> game;
    Renderer renderer {};
};


#endif //GAEMI_ENGINE_H
