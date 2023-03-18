//
// Created by gaetz on 07/11/2022.
//

#ifndef SUNSET_REVERIES_COMPONENTS_H
#define SUNSET_REVERIES_COMPONENTS_H

#include <optional>
#include <vector>
#include "raylib.h"
#include "Defines.h"
#include "AssetsManager.h"
#include "GMath.h"
#include "Renderer.h"

using std::vector;

enum class ComponentIndex {
    Transform2D = 0,
    Sprite = 1,
    Rigidbody2D = 2,
    BodyRaycast2D = 3
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

enum class Ray2DDirection {
    Left, Right, Down, Up
};

struct RigidbodyRaycast2D {
    RigidbodyRaycast2D(u32 entityId, const Rigidbody2D& attachBody,
                       i32 horizontalRaysCount, i32 verticalRaysCount,
                       f32 horizontalRayLength, f32 verticalRayLength, f32 margin
                       ):
        entityId { entityId }, attachBody { attachBody },
        verticalRaysCount { verticalRaysCount }, horizontalRaysCount { horizontalRaysCount },
        horizontalRayLength {horizontalRayLength}, verticalRayLength { verticalRayLength }, margin { margin }
    {
        if (verticalRaysCount < 2) {
            verticalRaysCount = 2;
        }
        if (horizontalRaysCount < 2) {
            horizontalRaysCount = 2;
        }
        verticalRays.clear();
        verticalRays = std::move(UpdateVerticalRays());
        horizontalRays.clear();
        horizontalRays = std::move(UpdateHorizontalRays());
    }

    u32 entityId;
    const Rigidbody2D& attachBody;
    i32 horizontalRaysCount;
    i32 verticalRaysCount;
    f32 horizontalRayLength;
    f32 verticalRayLength;
    f32 margin;
    Ray2DDirection currentVerticalDirection { Ray2DDirection::Down };
    Ray2DDirection currentHorizontalDirection { Ray2DDirection::Right };

    vector<Ray2D> verticalRays;
    vector<Ray2D> horizontalRays;

    void SetRayDirection(Ray2DDirection direction) {
        switch (direction) {
            case Ray2DDirection::Left:
                if (currentHorizontalDirection == Ray2DDirection::Left) return;
                currentHorizontalDirection = Ray2DDirection::Left;
                break;
            case Ray2DDirection::Right:
                if (currentHorizontalDirection == Ray2DDirection::Right) return;
                currentHorizontalDirection = Ray2DDirection::Right;
                break;
            case Ray2DDirection::Down:
                if (currentVerticalDirection == Ray2DDirection::Down) return;
                currentVerticalDirection = Ray2DDirection::Down;
                break;
            case Ray2DDirection::Up:
                if (currentVerticalDirection == Ray2DDirection::Up) return;
                currentVerticalDirection = Ray2DDirection::Up;
                break;
        }
    }

    std::vector<Ray2D> UpdateHorizontalRays() {
        std::vector<Ray2D> rays;
        const Vector2 startPosition { attachBody.pos + Vector2 { attachBody.boundingBox.x, attachBody.boundingBox.y } };
        const Vector2 endPosition = startPosition + Vector2 { 0, attachBody.boundingBox.height };
        const Vector2 offset = (endPosition - startPosition) / static_cast<f32>(horizontalRaysCount - 1);
        if (currentHorizontalDirection == Ray2DDirection::Left) {
            for (i32 i = 0; i < horizontalRaysCount; ++i) {
                rays.emplace_back(
                        startPosition + offset * i + Vector2 {margin, 0 },
                        Vector2 { -1, 0 },
                        horizontalRayLength + margin);
            }
        } else {
            for (i32 i = 0; i < horizontalRaysCount; ++i) {
                rays.emplace_back(
                        startPosition + offset * i - Vector2 {margin, 0 } + Vector2 { attachBody.boundingBox.width, 0 },
                        Vector2 { 1, 0 },
                        horizontalRayLength + margin);
            }
        }
        return rays;
    }

    std::vector<Ray2D> UpdateVerticalRays() {
        std::vector<Ray2D> rays;
        const Vector2 startPosition { attachBody.pos + Vector2 { attachBody.boundingBox.x, attachBody.boundingBox.y } };
        const Vector2 endPosition = startPosition + Vector2 { attachBody.boundingBox.width, 0 };
        const Vector2 offset = (endPosition - startPosition) / static_cast<f32>(verticalRaysCount - 1);
        if (currentVerticalDirection == Ray2DDirection::Down) {
            for (i32 i = 0; i < verticalRaysCount; ++i) {
                rays.emplace_back(
                        startPosition + offset * i - Vector2 { 0, margin } + Vector2 { 0, attachBody.boundingBox.height },
                        Vector2 { 0, 1 },
                        verticalRayLength + margin);
            }
        } else {
            for (i32 i = 0; i < verticalRaysCount; ++i) {
                rays.emplace_back(
                        startPosition + offset * i + Vector2 { 0, margin },
                        Vector2 { 0, -1 },
                        verticalRayLength + margin);
            }
        }
        return rays;
    }

    void Update() {
        for (auto& vRay : verticalRays) {
            verticalRays.clear();
            verticalRays = std::move(UpdateVerticalRays());
        }
        for (auto& hRay : horizontalRays) {
            horizontalRays.clear();
            horizontalRays = std::move(UpdateHorizontalRays());
        }
    }

    void DrawRays() {
        const Rectangle box { attachBody.pos.x + attachBody.boundingBox.x,
                        attachBody.pos.y + attachBody.boundingBox.y,
                        attachBody.boundingBox.width, attachBody.boundingBox.height };
        DrawRectangleLines(box.x, box.y, box.width, box.height, RED);
        for (auto& vRay : verticalRays) {
            render::DrawLine(vRay.origin, vRay.direction, vRay.length, RED);
        }
        for (auto& hRay : horizontalRays) {
            render::DrawLine(hRay.origin, hRay.direction, hRay.length, RED);
        }
    }
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
