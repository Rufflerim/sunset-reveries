//
// Created by gaetz on 05/11/2022.
//

#include "../gaemi/Engine.h"
#include "Game.h"
#include "Log.h"

int main()
{
    Log::Restart();

    Engine engine;
    engine.Start(1280, 720, "Sunset Reveries", std::make_unique<Game>());

    return 0;
}
