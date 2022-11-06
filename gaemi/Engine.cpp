//
// Created by gaetz on 05/11/2022.
//

#include "Engine.h"
#include "raylib.h"
#include "Window.h"
#include "IGame.h"
#include "Log.h"

void Engine::Start(i32 windowWidth, i32 windowHeight, const str& gameNameP,
                   unique_ptr<IGame>&& gameP) noexcept
{
    gameName = gameNameP;
    game = std::move(gameP);

    Window window { windowWidth, windowHeight, gameName };
    LOG(LogLevel::Info) << "Window initialized";

    SetTargetFPS(60);
    game->Load();
    LOG(LogLevel::Info) << "Game loaded";

    while (!window.ShouldClose())
    {
        game->Update(0.017f);

        renderer.BeginDraw();
        renderer.ClearScreen();
        game->Draw(renderer);
        renderer.EndDraw();
    }

    game->Unload();
    LOG(LogLevel::Info) << "Bye :)";
}
