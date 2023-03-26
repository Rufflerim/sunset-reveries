//
// Created by gaetz on 05/11/2022.
//

#include <algorithm>

#include "Engine.hpp"
#include "raylib.h"
#include "Window.hpp"
#include "IGame.hpp"
#include "Log.hpp"
#include "AssetsManager.hpp"

void Engine::Start(i32 windowWidth, i32 windowHeight, const str& gameNameP,
                   unique_ptr<IGame>&& gameP) noexcept
{
    gameName = gameNameP;
    game = std::move(gameP);

    Window window { windowWidth, windowHeight, gameName };
    SetTargetFPS(60);
    LOG(LogLevel::Info) << "Window initialized";

    AssetsManager::LoadData();
    game->Load();
    LOG(LogLevel::Info) << "Game loaded";

    while (!Window::ShouldClose())
    {
#ifdef GDEBUG
        f32 dt = std::min(GetFrameTime(), 0.0166f);
#else
        f32 dt = GetFrameTime();
#endif
        game->Update(dt);

        render::BeginDraw();
        render::ClearScreen();
        game->Draw();
        DrawFPS(50, 50);
        render::EndDraw();
    }

    game->Unload();
    LOG(LogLevel::Info) << "Bye :)";
}
