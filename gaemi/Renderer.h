//
// Created by gaetz on 05/11/2022.
//

#ifndef GAEMI_RENDERER_H
#define GAEMI_RENDERER_H

#include "Defines.h"

struct Texture;
struct Color;
struct Rectangle;

namespace render {

    void BeginDraw();

    void ClearScreen();

    void EndDraw();

    GAPI void DrawTexture(Texture texture2D, i32 x, i32 y, Color tint);
    GAPI void DrawSprite(Texture texture2D, Rectangle srcRect, Rectangle dstRect, Color tint);

}


#endif //GAEMI_RENDERER_H
