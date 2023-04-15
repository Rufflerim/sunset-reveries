//
// Created by gaetz on 05/03/23.
//

#ifndef SUNSET_REVERIES_WORLD_CHANGE_HPP
#define SUNSET_REVERIES_WORLD_CHANGE_HPP

struct PlayerChange {
    u64 entityId { 0 };
    Vector2 velocityDelta { 0, 0 };
};

struct PositionChange {
    u64 entityId { 0 };
    bool isGrounded { false };
    Vector2 velocityDelta { 0.0f, 0.0f };
    f32 dt { 0.0f };

    Vector2 groundVelocity {0.0f, 0.0f };
    f32 positionXFixAfterCollision { -1.0f };
    f32 positionYFixAfterCollision { -1.0f };
    bool stopVelocityX { false };
    bool stopVelocityY { false };
};

struct Rigidbody2D;
struct RaycastCollision {
    RaycastCollision(u64 entityIdP, u64 otherIdP, const Vector2& contactPointP,
                     const Rigidbody2D& emitterBodyP, const Rigidbody2D& otherBodyP, const Ray2D& rayP,
                    f32 lengthSquaredBeforeCollisionP)
    : entityId {entityIdP }, otherId { otherIdP }, contactPoint {contactPointP },
      emitterBody { emitterBodyP }, otherBody { otherBodyP }, ray { rayP },
      lengthSquaredBeforeCollision { lengthSquaredBeforeCollisionP }
    {}

    u64 entityId { 0 };
    u64 otherId { 0 };
    Vector2 contactPoint { 0, 0 };
    const Rigidbody2D& emitterBody;
    const Rigidbody2D& otherBody;
    Ray2D ray { {0, 0}, { 1 , 0}, 1.0f };
    f32 lengthSquaredBeforeCollision { 0 };
};

#endif //SUNSET_REVERIES_WORLD_CHANGE_HPP
