//
// Created by gaetz on 07/11/2022.
//

#ifndef SUNSET_REVERIES_COMPONENTS_HPP
#define SUNSET_REVERIES_COMPONENTS_HPP

#include <optional>
#include <utility>

#include "raylib.h"
#include "Defines.hpp"
#include "AssetsManager.hpp"
#include "Renderer.hpp"
#include "WorldChange.hpp"
#include "EntityRingBuffer.hpp"
#include "Ray2D.hpp"

class ECSManager;

using gmath::Ray2D;
using gmath::Vec2;
using gmath::Rect;

enum class ComponentIndex {
    Transform2D = 0,
    Sprite = 1,
    Rigidbody2D = 2,
    BodyRaycast2D = 3,
    Replay = 4,
    Weapon = 5
};

struct Transform2D {
    explicit Transform2D(u64 entityIdP) : entityId { entityIdP } {}

    u64 entityId;
    Vec2 pos { 0.0f, 0.0f };
    float rotation { 0.0f };
    Vec2 scale { 0.0f, 0.0f };
};

struct Sprite {
    explicit Sprite(u64 entityIdP, const str& textNameP, float width, float height) :
        entityId { entityIdP },
        srcRect { 0, 0, width, height },
        dstRect { 0, 0, width, height },
        texName { textNameP },
        tex { AssetsManager::GetTexture(textNameP) }
    {}

    u64 entityId;
    u8 opacity { 255 };
    Rect srcRect { 0, 0, 1, 1 };
    Rect dstRect { 0, 0, 1, 1 };
    str texName;
    Texture tex;
};

struct Rigidbody2D {
    explicit Rigidbody2D(u64 entityIdP, const Vec2& pos, const Rect& box,
                         bool doApplyGravityP, bool isGhostP) :
        entityId { entityIdP },
        pos { pos },
        boundingBox { box },
        doApplyGravity { doApplyGravityP },
        isGhost { isGhostP }
    {}

    u64 entityId;
    Vec2 pos { 0.0f, 0.0f };
    Rect boundingBox { 0, 0, 1, 1 };
    Vec2 velocity { 0.0f, 0.0f };
    Vec2 playerAcceleration {0.0f, 0.0f };
    Vec2 groundVelocity {0.0f, 0.0f };
    bool doApplyGravity { true };
    bool isGhost { false };
    bool isGrounded { false };

    [[nodiscard]] Rect GetPositionedRect() const {
        return Rect { pos.x + boundingBox.x, pos.y + boundingBox.y, boundingBox.width, boundingBox.height };
    }

    [[nodiscard]] f32 GetRealX() const {
        return pos.x + boundingBox.x;
    }

    [[nodiscard]] f32 GetRealY() const {
        return pos.y + boundingBox.y;
    }

#ifdef GDEBUG
    void DrawDebug() const {
        const Rect box { pos.x + boundingBox.x,
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
    RigidbodyRaycast2D(u64 entityId, sptr<ECSManager> ecsP,
                       i32 horizontalRaysCountP, i32 verticalRaysCountP,
                       f32 horizontalRayLength, f32 verticalRayLength, f32 margin);

    u64 entityId;
    Rigidbody2D attachBody { 0, Vec2  { 0, 0 }, Rect  { 0, 0, 1, 1 }, false, false };
    i32 horizontalRaysCount;
    i32 verticalRaysCount;
    f32 horizontalRayLength;
    f32 verticalRayLength;
    f32 margin;
    Ray2DDirection currentVerticalDirection { Ray2DDirection::Down };
    Ray2DDirection currentHorizontalDirection { Ray2DDirection::Right };
    sptr<ECSManager> ecs;

    vector<Ray2D> verticalRays;
    vector<Ray2D> horizontalRays;

    void SetRayDirection(Ray2DDirection direction);
    vector<Ray2D> UpdateHorizontalRays();
    vector<Ray2D> UpdateVerticalRays();
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
    Replay(u64 entityIdP, u64 originalEntityIdP, u32 replayStartFrameP, u32 replayEndFrameP)
        : entityId { entityIdP }, originalEntityId { originalEntityIdP },
        replayStartFrame { replayStartFrameP }, replayEndFrame { replayEndFrameP }
    {}

    u64 entityId;
    u64 originalEntityId { 0 };
    u32 replayStartFrame { 0 };
    u32 replayEndFrame { 0 };
    u32 currentFrame { 0 };
    vector<Transform2D> transforms;
    vector<Rigidbody2D> bodies;
};

enum class CharacterOrientation {
    Right, Left, Top, Bottom
};

struct Weapon {
    Weapon(u64 entityIdP, EntityRingBuffer* projectileBufferP)
    : entityId { entityIdP }, projectileBuffer { projectileBufferP } {

    }

    u64 entityId;
    Vec2 projectileOriginOffset { 20.0f, 0.0f};
    i32 projectilePerShoot { 1 };
    f32 angle { 0.0f };
    i32 angularSpreadDegree { 10 };
    f32 cooldown { 0.05f };
    bool isOneShot { false };
    bool doesGravityAffectProjectile { false };
    CharacterOrientation currentOrientation { CharacterOrientation::Right };
    //sptr<ECSManager> ecs;
    EntityRingBuffer* projectileBuffer;

    void Update(f32 dt);
    void Shoot();
    void ShootOnce() const;

    f32 cooldownTimer { 0.0f };
};

// Utils
struct Collision2D {
    Collision2D(u64 entityId, Rect currentBox, Vec2 velocity,
                u64 otherId, Rect otherCurrentBox, Vec2 otherVelocity):
            entityId { entityId}, currentBox { currentBox }, velocity { velocity },
            otherId { otherId }, otherCurrentBox { otherCurrentBox }, otherVelocity { otherVelocity }
    {}

    u64 entityId;
    Rect currentBox;
    Vec2 velocity;
    u64 otherId;
    Rect otherCurrentBox;
    Vec2 otherVelocity;
};

#endif //SUNSET_REVERIES_COMPONENTS_HPP
