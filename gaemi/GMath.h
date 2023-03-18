//
// Created by gaetz on 07/03/23.
//

#ifndef SUNSET_REVERIES_GMATH_H
#define SUNSET_REVERIES_GMATH_H

#include "Defines.h"
#include "raylib.h"

Vector2 operator+(Vector2 lh, Vector2 rh);
Vector2 operator-(Vector2 lh, Vector2 rh);

Vector2 operator*(Vector2 v, f32 f);
Vector2 operator*(f32 f, Vector2 v);

Vector2 operator/(Vector2 v, f32 f);
Vector2 operator/(f32 f, Vector2 v);

struct Ray2D
{
    Ray2D(Vector2 origin, Vector2 direction, f32 length):
        origin{origin}, direction{direction}, length{length}
    {}

    Vector2 origin{};
    Vector2 direction{};
    f32 length { -1 };
};

bool RayIntersectRect(Rectangle rect, Ray2D ray, Vector2& outContactPoint);

#endif //SUNSET_REVERIES_GMATH_H
