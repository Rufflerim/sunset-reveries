//
// Created by gaetz on 05/11/2022.
//

#include "Renderer.h"
#include "raylib.h"

void Renderer::BeginDraw() {
    BeginDrawing();
}

void Renderer::ClearScreen() {
    ClearBackground(BLACK);
}

void Renderer::EndDraw() {
    EndDrawing();
}