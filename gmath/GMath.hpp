//
// Created by gaetz on 07/03/23.
//

#ifndef GMATH_GMATH_HPP
#define GMATH_GMATH_HPP

#include "raylib.h"
#include "Types.hpp"
#include <array>


namespace gmath {

    // Trigonometry

    f32 ToDeg(f32 angleRadians);
    f32 ToRad(f32 angleDegrees);

    f32 CosRad(f32 angleRadians);
    f32 CosDeg(f32 angleDegrees);
    f32 Cos(f32 angleDegrees);

    f32 SinRad(f32 angleRadians);
    f32 SinDeg(f32 angleDegrees);
    f32 Sin(f32 angleDegrees);

    f32 TanRad(f32 angleRadians);
    f32 TanDeg(f32 angleDegrees);
    f32 Tan(f32 angleDegrees);

    f32 CoTanRad(f32 angleRadians);
    f32 CoTanDeg(f32 angleDegrees);
    f32 CoTan(f32 angleDegrees);

    f32 Acos(f32 value);
    f32 Asin(f32 value);
    f32 Atan2(f32 y, f32 x);


    // Approximations

    template<typename T>
    T Max(const T& a, const T& b) {
        return (a < b ? b : a);
    }

    template<typename T>
    T Min(const T& a, const T& b) {
        return (a < b ? a : b);
    }

    template<typename T>
    T Clamp(const T& value, const T& lower, const T& upper) {
        return Min(upper, Max(lower, value));
    }

    i32 Round(f32 value);
    f32 Abs(f32 value);
    i32 Floor(f32 value);
    i32 Ceil(f32 value);
    f32 Pow(f32 base, f32 exponent);


    // Easing

    /***
     * Easing function. Grow quick then reach limit slowly.
     * @param t From 0 to 1
     * @return Eased value from 0 to 1.
     */
    f32 EaseOutExpo(f32 t);

}

#endif //GMATH_GMATH_HPP
