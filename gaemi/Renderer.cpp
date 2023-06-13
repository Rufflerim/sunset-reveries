//
// Created by gaetz on 05/11/2022.
//

#include <cmath>
#include "Renderer.hpp"
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

    void DrawSprite(Texture texture2D, Rect srcRect, Rect dstRect, Color tint) {
        ::DrawTexturePro(texture2D, srcRect.ToRaylib(), dstRect.ToRaylib(), { 0, 0 }, 0, tint);
    }

    void DrawLine(const Vec2& origin, const Vec2& direction, f32 length, Color color) {
        const Vec2 end = (origin + (direction * length));
        ::DrawLine(gmath::Round(origin.x), gmath::Round(origin.y),
                   gmath::Round(end.x), gmath::Round(end.y), color);
    }

    void DrawRectangleLines(const Rect& rect, Color color) {
        Rect rectInt = rect.ToRectInt();
        ::DrawRectangleLines(static_cast<i32>(rectInt.x), static_cast<i32>(rectInt.y),
                             static_cast<i32>(rectInt.width), static_cast<i32>(rectInt.height), color);
    }

    void DrawDefaultText(const str& text, const Vec2& pos, i32 fontSize, Color color) {
        ::DrawText(text.c_str(), gmath::Round(pos.x), gmath::Round(pos.y), fontSize, color);
    }

}