//
// Created by gaetz on 05/11/2022.
//

#ifndef GAEMI_RENDERER_HPP
#define GAEMI_RENDERER_HPP

#include "Defines.hpp"

struct Texture;
struct Color;
struct Rectangle;
struct Vector2;

namespace render {

    void BeginDraw();

    void ClearScreen();

    void EndDraw();

    GAPI void DrawTexture(Texture texture2D, i32 x, i32 y, Color tint);
    GAPI void DrawSprite(Texture texture2D, Rectangle srcRect, Rectangle dstRect, Color tint);
    GAPI void DrawLine(const Vector2& origin, const Vector2& direction, float length, Color color);
    GAPI void DrawRectangleLines(const Rectangle& rect, Color color);
    GAPI void DrawDefaultText(const str& text, const Vector2& pos, i32 fontSize, Color color);

}


#endif //GAEMI_RENDERER_HPP
