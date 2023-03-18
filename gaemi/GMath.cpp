//
// Created by gaetz on 12/03/2023.
//

#include "GMath.h"

Vector2 operator+(Vector2 lh, Vector2 rh) {
    Vector2 res { lh.x + rh.x, lh.y + rh.y };
    return res;
}

Vector2 operator-(Vector2 lh, Vector2 rh) {
    Vector2 res { lh.x - rh.x, lh.y - rh.y };
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

Vector2 operator/(Vector2 v, f32 f) {
    Vector2 res { v.x / f, v.y / f };
    return res;
}

Vector2 operator/(f32 f, Vector2 v) {
    Vector2 res { v.x / f, v.y / f };
    return res;
}

bool RayIntersectRect(Rectangle rect, Ray2D ray, Vector2& outContactPoint)
{
    f32 minParam = std::numeric_limits<f32>::min();
    f32 maxParam = std::numeric_limits<f32>::max();

    if (ray.direction.x != 0.0) {
        f32 txMin = (rect.x - ray.origin.x) / ray.direction.x;
        f32 txMax = ((rect.x + rect.width) - ray.origin.x) / ray.direction.x;

        minParam = std::max(minParam, std::min(txMin, txMax));
        maxParam = std::min(maxParam, std::max(txMin, txMax));
    }

    if (ray.direction.y != 0.0) {
        f32 tyMin = (rect.y - ray.origin.y) / ray.direction.y;
        f32 tyMax = ((rect.y + rect.height) - ray.origin.y) / ray.direction.y;

        minParam = std::max(minParam, std::min(tyMin, tyMax));
        maxParam = std::min(maxParam, std::max(tyMin, tyMax));
    }

    // If maxParam < 0, ray is intersecting AABB, but the whole AABB is behind us
    if (maxParam < 0) {
        return false;
    }

    // If minParam > maxParam, ray doesn't intersect AABB
    if (minParam > maxParam) {
        return false;
    }

    // If minParam > ray.length, the collision occurs after ray length
    if (ray.length > 0 && minParam > ray.length) {
        return false;
    }

    outContactPoint = ray.origin + (ray.direction * minParam);
    return true;
}