//
// Created by gaetz on 07/11/2022.
//

#ifndef SUNSET_REVERIES_COMPONENTS_H
#define SUNSET_REVERIES_COMPONENTS_H

#include "raylib.h"
#include "Defines.h"
#include "AssetsManager.h"

enum class ComponentIndices {
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
        tex { const_cast<Texture&>(AssetsManager::GetTexture(textNameP)) }
    {}

    Sprite(Sprite&& other) noexcept :
            entityId { other.entityId },
            srcRect { other.srcRect },
            dstRect { other.dstRect },
            texName { other.texName },
            tex { const_cast<Texture&>(AssetsManager::GetTexture(texName)) }
    {

    }

    Sprite& operator=(Sprite&& other) noexcept {
        entityId = other.entityId;
        srcRect = other.srcRect;
        dstRect = other.dstRect;
        texName = other.texName;
        tex = AssetsManager::GetTexture(texName);
        return *this;
    };

    u32 entityId;
    Rectangle srcRect { 0, 0, 1, 1 };
    Rectangle dstRect { 0, 0, 1, 1 };
    str texName;
    Texture& tex;
};

struct Rigidbody2D {
    explicit Rigidbody2D(u32 entityIdP, const Rectangle& box) :
        entityId { entityIdP },
        boundingBox { box }
    {}

    u32 entityId;
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
