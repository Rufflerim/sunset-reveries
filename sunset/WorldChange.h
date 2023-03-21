//
// Created by gaetz on 05/03/23.
//

#ifndef SUNSET_REVERIES_WORLDCHANGE_H
#define SUNSET_REVERIES_WORLDCHANGE_H

struct PlayerChange {
    u32 entityId;
    Vector2 velocityDelta;
};

struct CollisionChange {
    u32 entityId;
    Vector2 positionDelta;
    Vector2 newVelocity;
};

struct PositionChange {
    u32 entityId;
    bool isGrounded;
    Vector2 positionDelta { 0.0f, 0.0f };
    Vector2 velocityDelta { 0.0f, 0.0f };
};

struct BounceChange {
    u32 entityId;
    f32 newY;
    f32 newVerticalVelocity;
};

struct RaycastCollision {
    RaycastCollision(u32 entityIdP, u32 otherIdP, const Vector2& contactPointP,
                     const Rigidbody2D& emitterBodyP, const Rigidbody2D& otherBodyP, const Ray2D& rayP,
                    f32 lengthSquaredBeforeCollisionP)
    : entityId {entityIdP }, otherId { otherIdP }, contactPoint {contactPointP },
      emitterBody { emitterBodyP }, otherBody { otherBodyP }, ray { rayP },
      lengthSquaredBeforeCollision { lengthSquaredBeforeCollisionP }
    {}

    u32 entityId;
    u32 otherId;
    Vector2 contactPoint;
    const Rigidbody2D& emitterBody;
    const Rigidbody2D& otherBody;
    Ray2D ray;
    f32 lengthSquaredBeforeCollision;
};

#endif //SUNSET_REVERIES_WORLDCHANGE_H
