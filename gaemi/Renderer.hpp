//
// Created by gaetz on 05/11/2022.
//

#ifndef GAEMI_RENDERER_HPP
#define GAEMI_RENDERER_HPP

#include "Defines.hpp"
#include "Rect.hpp"

struct Texture;
struct Color;

using gmath::Vec2;
using gmath::Rect;

namespace render {

    void BeginDraw();

    void ClearScreen();

    void EndDraw();

    GAPI void DrawTexture(Texture texture2D, i32 x, i32 y, Color tint);
    GAPI void DrawSprite(Texture texture2D, Rect srcRect, Rect dstRect, Color tint);
    GAPI void DrawLine(const Vec2& origin, const Vec2& direction, float length, Color color);
    GAPI void DrawRectangleLines(const Rect& rect, Color color);
    GAPI void DrawDefaultText(const str& text, const Vec2& pos, i32 fontSize, Color color);

}


#endif //GAEMI_RENDERER_HPP
