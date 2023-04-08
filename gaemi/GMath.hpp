//
// Created by gaetz on 07/03/23.
//

#ifndef SUNSET_REVERIES_GMATH_HPP
#define SUNSET_REVERIES_GMATH_HPP

#include "Defines.hpp"
#include "raylib.h"
#include <array>
#include <cmath>

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

    Vector2 origin {};
    Vector2 direction {};
    f32 length { -1 };
};

bool RayVsRect2D(const Vector2& rayOrigin, const Vector2& rayDir, const Rectangle& targetRect, Vector2& contactPoint, Vector2& contactNormal, float& nearContactTime, std::array<Vector2, 2>& probableContactPoints);

f32 EaseOutExpo(f32 t);

#endif //SUNSET_REVERIES_GMATH_HPP
