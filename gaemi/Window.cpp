//
// Created by gaetz on 05/11/2022.
//

#include "Window.h"
#include "raylib.h"


Window::Window(i32 width, i32 height, str titleP):
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
