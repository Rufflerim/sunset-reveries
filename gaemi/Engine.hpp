//
// Created by gaetz on 05/11/2022.
//

#ifndef GAEMI_ENGINE_HPP
#define GAEMI_ENGINE_HPP

#include <string>
#include "Renderer.hpp"
#include "Defines.hpp"

class IGame;

class Engine {
public:
    Engine() = default;
    ~Engine() = default;
    Engine(Engine&) = delete;
    Engine& operator=(Engine&) = delete;

    GAPI void Start(i32 windowWidth, i32 windowHeight, const str& gameName,
                                     uptr<IGame>&& gameP) noexcept;

    str gameName;
    uptr<IGame> game;
};


#endif //GAEMI_ENGINE_HPP
