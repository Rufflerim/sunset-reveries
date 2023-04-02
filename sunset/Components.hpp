//
// Created by gaetz on 07/11/2022.
//

#ifndef SUNSET_REVERIES_COMPONENTS_HPP
#define SUNSET_REVERIES_COMPONENTS_HPP

#include <optional>
#include <vector>
#include <memory>

#include "raylib.h"
#include "Defines.hpp"
#include "AssetsManager.hpp"
#include "GMath.hpp"
#include "Renderer.hpp"
#include "WorldChange.hpp"

class ECSManager;
using std::vector;

enum class ComponentIndex {
    Transform2D = 0,
    Sprite = 1,
    Rigidbody2D = 2,
    BodyRaycast2D = 3,
    Replay = 4
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
    u8 opacity { 255 };
    Rectangle srcRect { 0, 0, 1, 1 };
    Rectangle dstRect { 0, 0, 1, 1 };
    str texName;
    Texture tex;
};

struct Rigidbody2D {
    explicit Rigidbody2D(u32 entityIdP, const Vector2& pos, const Rectangle& box,
                         bool doApplyGravityP, bool isGhostP) :
        entityId { entityIdP },
        pos { pos },
        boundingBox { box },
        doApplyGravity { doApplyGravityP },
        isGhost { isGhostP }
    {}

    u32 entityId;
    Vector2 pos { 0.0f, 0.0f };
    Rectangle boundingBox { 0, 0, 1, 1 };
    Vector2 velocity { 0.0f, 0.0f };
    Vector2 playerAcceleration {0.0f, 0.0f };
    Vector2 groundVelocity {0.0f, 0.0f };
    bool doApplyGravity { true };
    bool isGhost { false };
    bool isGrounded { false };

    [[nodiscard]] Rectangle GetPositionedRectangle() const {
        return Rectangle { pos.x + boundingBox.x, pos.y + boundingBox.y, boundingBox.width, boundingBox.height };
    }

    [[nodiscard]] f32 GetRealX() const {
        return pos.x + boundingBox.x;
    }

    [[nodiscard]] f32 GetRealY() const {
        return pos.y + boundingBox.y;
    }

#ifdef GDEBUG
    void DrawDebug() {
        const Rectangle box { pos.x + boundingBox.x,
                              pos.y + boundingBox.y,
                              boundingBox.width, boundingBox.height };
        render::DrawRectangleLines(box, BLUE);
    }
#endif
};

enum class Ray2DDirection {
    Left, Right, Down, Up
};

struct RigidbodyRaycast2D {
    RigidbodyRaycast2D(u32 entityId, std::shared_ptr<ECSManager> ecsP,
                       i32 horizontalRaysCountP, i32 verticalRaysCountP,
                       f32 horizontalRayLength, f32 verticalRayLength, f32 margin);

    u32 entityId;
    Rigidbody2D attachBody { 0, Vector2  { 0, 0 }, Rectangle  { 0, 0, 1, 1 }, false, false };
    i32 horizontalRaysCount;
    i32 verticalRaysCount;
    f32 horizontalRayLength;
    f32 verticalRayLength;
    f32 margin;
    Ray2DDirection currentVerticalDirection { Ray2DDirection::Down };
    Ray2DDirection currentHorizontalDirection { Ray2DDirection::Right };
    std::shared_ptr<ECSManager> ecs;

    vector<Ray2D> verticalRays;
    vector<Ray2D> horizontalRays;

    void SetRayDirection(Ray2DDirection direction);
    std::vector<Ray2D> UpdateHorizontalRays();
    std::vector<Ray2D> UpdateVerticalRays();
    void Update();

#ifdef GDEBUG
    void DrawDebug() {
        for (auto& vRay : verticalRays) {
            render::DrawLine(vRay.origin, vRay.direction, vRay.length, BLUE);
        }
        for (auto& hRay : horizontalRays) {
            render::DrawLine(hRay.origin, hRay.direction, hRay.length, BLUE);
        }
    }
#endif
};

struct Replay {
    Replay(u32 entityIdP, u32 originalEntityIdP, u32 replayStartFrameP, u32 replayEndFrameP)
        : entityId { entityIdP }, originalEntityId { originalEntityIdP },
        replayStartFrame { replayStartFrameP }, replayEndFrame { replayEndFrameP }
    {}

    u32 entityId;
    u32 originalEntityId { 0 };
    u32 replayStartFrame { 0 };
    u32 replayEndFrame { 0 };
    u32 currentFrame { 0 };
    vector<Transform2D> transforms;
    vector<Rigidbody2D> bodies;
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

#endif //SUNSET_REVERIES_COMPONENTS_HPP
