//
// Created by gaetz on 05/11/2022.
//

#include "../gaemi/Engine.h"
#include "Game.h"

int main()
{
    unique_ptr<Game> game = std::make_unique<Game>();

    Engine engine;
    engine.Start(1280, 720, "Sunset Reveries", reinterpret_cast<unique_ptr<IGame>&&>(game));

    return 0;
}