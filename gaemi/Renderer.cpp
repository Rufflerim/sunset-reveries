//
// Created by gaetz on 05/11/2022.
//

#include <cmath>
#include "Renderer.hpp"
#include "raylib.h"
#include "GMath.hpp"

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

    void DrawSprite(Texture texture2D, Rectangle srcRect, Rectangle dstRect, Color tint) {
        ::DrawTexturePro(texture2D, srcRect, dstRect, { 0, 0 }, 0, tint);
    }

    void DrawLine(const Vector2& origin, const Vector2& direction, float length, Color color) {
        const Vector2 end = (origin + (direction * length));
        ::DrawLine(static_cast<i32>(std::round(origin.x)), static_cast<i32>(std::round(origin.y)),
                   static_cast<i32>(std::round(end.x)), static_cast<i32>(std::round(end.y)), color);
    }

    void DrawRectangleLines(const Rectangle& rect, Color color) {
        ::DrawRectangleLines(static_cast<i32>(rect.x), static_cast<i32>(rect.y),
                             static_cast<i32>(rect.width), static_cast<i32>(rect.height), color);
    }
}