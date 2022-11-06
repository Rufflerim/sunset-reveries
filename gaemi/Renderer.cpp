//
// Created by gaetz on 05/11/2022.
//

#include "Renderer.h"
#include "raylib.h"

namespace render {
    void BeginDraw() {
        BeginDrawing();
    }

    void ClearScreen() {
        ClearBackground(BLACK);
    }

    void EndDraw() {
        EndDrawing();
    }

    void DrawTexture(Texture texture, i32 x, i32 y, Color tint) {
        ::DrawTexture(texture, x, y, tint);
    }
}