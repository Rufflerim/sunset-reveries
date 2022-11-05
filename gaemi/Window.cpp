//
// Created by gaetz on 05/11/2022.
//

#include "Window.h"
#include "raylib.h"


Window::Window(int width, int height, std::string titleP):
    width { width },
    height { height },
    title { std::move(titleP) }
{
    InitWindow(width, height, title.c_str());
}

Window::~Window() {
    CloseWindow();
}

bool Window::ShouldClose() {
    return WindowShouldClose();
}
