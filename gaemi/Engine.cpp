//
// Created by gaetz on 05/11/2022.
//

#include "Engine.h"
#include "raylib.h"
#include "Window.h"
#include "IGame.h"

void Engine::Start(int windowWidth, int windowHeight, const std::string& gameNameP,
                   unique_ptr<IGame> gameP)
{
    gameName = gameNameP;
    game = std::move(gameP);

    Window window { windowWidth, windowHeight, gameName };
    SetTargetFPS(60);

    game->Load();
    while (!window.ShouldClose())
    {
        game->Update(0);

        renderer.BeginDraw();
        renderer.ClearScreen();
        game->Draw(renderer);
        renderer.EndDraw();
    }
}
