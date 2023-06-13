//
// Created by gaetz on 12/03/2023.
//

#include "GMath.hpp"
#include "Vec2.hpp"

#include <cmath>

namespace gmath {

    f32 ToDeg(f32 angleRadians) {
        return angleRadians * 360.0f / PI;
    }

    f32 ToRad(f32 angleDegrees) {
        return angleDegrees * PI / 360.0f;
    }

    f32 CosRad(f32 angleRadians) {
        return std::cos(angleRadians);
    }

    f32 CosDeg(f32 angleDegrees) {
        const f32 angleRadians = ToRad(angleDegrees);
        return std::cos(angleRadians);
    }

    f32 Cos(f32 angleDegrees) {
        return CosDeg(angleDegrees);
    }

    f32 SinRad(f32 angleRadians) {
        return std::sin(angleRadians);
    }

    f32 SinDeg(f32 angleDegrees) {
        const f32 angleRadians = ToRad(angleDegrees);
        return std::sin(angleRadians);
    }

    f32 Sin(f32 angleDegrees) {
        return SinDeg(angleDegrees);
    }

    f32 TanRad(f32 angleRadians) {
        return std::tan(angleRadians);
    }

    f32 TanDeg(f32 angleDegrees) {
        const f32 angleRadians = ToRad(angleDegrees);
        return std::tan(angleRadians);
    }

    f32 Tan(f32 angleDegrees) {
        return TanDeg(angleDegrees);
    }

    f32 CoTanRad(f32 angleRadians) {
        return 1.0f / std::tan(angleRadians);
    }

    f32 CoTanDeg(f32 angleDegrees) {
        const f32 angleRadians = ToRad(angleDegrees);
        return 1.0f / std::tan(angleRadians);
    }

    f32 CoTan(f32 angleDegrees) {
        return CoTanDeg(angleDegrees);
    }

    f32 Acos(f32 value) {
        return ToDeg(std::acos(value));
    }

    f32 Asin(f32 value) {
        return ToDeg(std::asin(value));
    }

    f32 Atan2(f32 y, f32 x) {
        return ToDeg(std::atan2(y, x));
    }

    i32 Round(f32 value) {
        return static_cast<i32>(round(value));
    }

    f32 Abs(f32 value) {
        return std::abs(value);
    }

    i32 Floor(f32 value) {
        return static_cast<i32>(floor(value));
    }

    i32 Ceil(f32 value) {
        return static_cast<i32>(ceil(value));
    }

    f32 EaseOutExpo(f32 t) {
        return t == 1.0f ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * t);
    }
}


