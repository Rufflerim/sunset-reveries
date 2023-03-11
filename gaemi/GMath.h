//
// Created by gaetz on 07/03/23.
//

#ifndef SUNSET_REVERIES_GMATH_H
#define SUNSET_REVERIES_GMATH_H

#include "Defines.h"
#include "raylib.h"

Vector2 operator+(Vector2 lh, Vector2 rh) {
    Vector2 res { lh.x + rh.x, lh.y + rh.y };
    return res;
}

Vector2 operator*(Vector2 v, f32 f) {
    Vector2 res { v.x * f, v.y * f };
    return res;
}

Vector2 operator*(f32 f, Vector2 v) {
    Vector2 res { v.x * f, v.y * f };
    return res;
}

#endif //SUNSET_REVERIES_GMATH_H
