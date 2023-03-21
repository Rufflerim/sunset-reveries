//
// Created by gaetz on 07/03/23.
//

#ifndef SUNSET_REVERIES_GMATH_H
#define SUNSET_REVERIES_GMATH_H

#include "Defines.h"
#include "raylib.h"
#include <array>

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

bool RayVsRect2D(const Vector2& ray_origin, const Vector2& ray_dir, const Rectangle& targetRect, Vector2& contact_point, Vector2& contact_normal, float& near_contact_time, std::array<Vector2, 2>& probableContactPoints);

#endif //SUNSET_REVERIES_GMATH_H
