//
// Created by gaetz on 05/03/23.
//

#ifndef SUNSET_REVERIES_WORLDCHANGE_H
#define SUNSET_REVERIES_WORLDCHANGE_H

struct CollisionChange {
    u32 entityId;
    Vector2 positionDelta;
    Vector2 newVelocity;
};

struct PositionChange {
    u32 entityId;
    Vector2 positionDelta;
};

struct BounceChange {
    u32 entityId;
    f32 newY;
    f32 newVerticalVelocity;
};

#endif //SUNSET_REVERIES_WORLDCHANGE_H
