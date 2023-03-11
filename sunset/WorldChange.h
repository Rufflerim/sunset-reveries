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

#endif //SUNSET_REVERIES_WORLDCHANGE_H
