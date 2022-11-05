//
// Created by gaetz on 05/11/2022.
//

#include "Engine.h"
#include "raylib.h"
#include "Window.h"

void Engine::Start(int windowWidth, int windowHeight, const std::string& gameNameP)
{
    gameName = gameNameP;

    Window window { windowWidth, windowHeight, gameName };
    SetTargetFPS(60);

    while (!window.ShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        EndDrawing();
    }
}
