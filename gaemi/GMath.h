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

struct Ray2D
{
    Vector2 origin;
    Vector2 direction;
};

bool RayIntersectRect(Rectangle rect, Ray2D ray, Vector2* point)
{
    float minParam = std::numeric_limits<f32>::min;
    f32 maxParam = std::numeric_limits<f32>::max;

    if (ray.direction.x != 0.0)
    {
        float txMin = (rect.x - ray.origin.x) / ray.direction.x;
        float txMax = ((rect.x + rect.width) - ray.origin.x) / ray.direction.x;

        minParam = fmax(minParam, fmin(txMin, txMax));
        maxParam = fmin(maxParam, fmax(txMin, txMax));
    }

    if (ray.direction.y != 0.0)
    {
        float tyMin = (rect.y - ray.origin.y) / ray.direction.y;
        float tyMax = ((rect.y + rect.height) - ray.origin.y) / ray.direction.y;

        minParam = fmax(minParam, fmin(tyMin, tyMax));
        maxParam = fmin(maxParam, fmax(tyMin, tyMax));
    }

    // if maxParam < 0, ray is intersecting AABB, but the whole AABB is behind us
    if (maxParam < 0)
    {
        return false;
    }

    // if minParam > maxParam, ray doesn't intersect AABB
    if (minParam > maxParam)
    {
        return false;
    }

    if (point != NULL)
    {
        *point = Vector2Add(ray.origin, Vector2Scale(ray.direction, minParam));
    }
    return true;
}

#endif //SUNSET_REVERIES_GMATH_H
