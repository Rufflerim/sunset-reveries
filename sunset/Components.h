//
// Created by gaetz on 07/11/2022.
//

#ifndef SUNSET_REVERIES_COMPONENTS_H
#define SUNSET_REVERIES_COMPONENTS_H

#include "raylib.h"
#include "Defines.h"
#include "AssetsManager.h"

enum class ComponentIndex {
    Transform2D = 0,
    Sprite = 1,
    Rigidbody2D = 2
};

struct Transform2D {
    explicit Transform2D(u32 entityIdP) : entityId { entityIdP } {}

    u32 entityId;
    Vector2 pos { 0.0f, 0.0f };
    float rotation { 0.0f };
    Vector2 scale { 0.0f, 0.0f };
};

struct Sprite {
    explicit Sprite(u32 entityIdP, const str& textNameP, float width, float height) :
        entityId { entityIdP },
        srcRect { 0, 0, width, height },
        dstRect { 0, 0, width, height },
        texName { textNameP },
        tex { AssetsManager::GetTexture(textNameP) }
    {}

    u32 entityId;
    Rectangle srcRect { 0, 0, 1, 1 };
    Rectangle dstRect { 0, 0, 1, 1 };
    str texName;
    Texture tex;
};

struct Rigidbody2D {
    explicit Rigidbody2D(u32 entityIdP, const Vector2& pos, const Rectangle& box) :
        entityId { entityIdP },
        pos { pos },
        boundingBox { box }
    {}

    u32 entityId;
    Vector2 pos { 0.0f, 0.0f };
    Rectangle boundingBox { 0, 0, 1, 1 };
    Vector2 velocity { 0, 0 };
};

// Utils

struct Collision2D {
    Collision2D(u32 entityId, Rectangle currentBox, Vector2 velocity,
                u32 otherId, Rectangle otherCurrentBox, Vector2 otherVelocity):
            entityId { entityId}, currentBox { currentBox }, velocity { velocity },
            otherId { otherId }, otherCurrentBox { otherCurrentBox }, otherVelocity { otherVelocity }
    {}

    u32 entityId;
    Rectangle currentBox;
    Vector2 velocity;
    u32 otherId;
    Rectangle otherCurrentBox;
    Vector2 otherVelocity;
};

#endif //SUNSET_REVERIES_COMPONENTS_H
