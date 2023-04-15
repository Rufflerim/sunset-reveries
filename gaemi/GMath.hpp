//
// Created by gaetz on 07/03/23.
//

#ifndef SUNSET_REVERIES_GMATH_HPP
#define SUNSET_REVERIES_GMATH_HPP

#include "Defines.hpp"
#include "raylib.h"
#include <array>

using std::array;

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

namespace gmath {

    /***
     * Check if a 2d ray collides a rectangle
     * @param rayOrigin
     * @param rayDir
     * @param targetRect
     * @param contactPoint
     * @param contactNormal
     * @param nearContactTime
     * @param probableContactPoints
     * @return True if the intersection exists. Details are computed in contactPoint, contactNormal and nearContactTime.
     */
    bool RayVsRect2D(const Vector2 &rayOrigin, const Vector2 &rayDir, const Rectangle &targetRect, Vector2 &contactPoint,
                Vector2 &contactNormal, float &nearContactTime, array<Vector2, 2> &probableContactPoints);

    /***
     * Easing function. Grow quick then reach limit slowly.
     * @param t From 0 to 1
     * @return Eased value from 0 to 1.
     */
    f32 EaseOutExpo(f32 t);

}

#endif //SUNSET_REVERIES_GMATH_HPP
