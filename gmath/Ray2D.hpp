//
// Created by gaetz on 13/06/2023.
//

#ifndef GMATH_RAY2D_HPP
#define GMATH_RAY2D_HPP

#include "Vec2.hpp"
#include "Rect.hpp"

namespace gmath {

    class Ray2D {
    public:
        Ray2D(Vec2 origin, Vec2 direction, f32 length);
        Vec2 origin;
        Vec2 direction;
        f32 length{-1};

    };

    /***
     * Check if a 2d ray collides a Rect
     * @param rayOrigin
     * @param rayDir
     * @param targetRect
     * @param contactPoint
     * @param contactNormal
     * @param nearContactTime
     * @param probableContactPoints
     * @return True if the intersection exists. Details are computed in contactPoint, contactNormal and nearContactTime.
     */
    bool RayVsRect2D(const Vec2 &rayOrigin, const Vec2 &rayDir, const Rect &targetRect, Vec2 &contactPoint,
                     Vec2 &contactNormal, float &nearContactTime, array<Vec2, 2> &probableContactPoints);

}


#endif //GMATH_RAY2D_HPP
