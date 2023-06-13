//
// Created by gaetz on 13/06/2023.
//

#ifndef GMATH_VEC2_HPP
#define GMATH_VEC2_HPP

#include "Types.hpp"
#include "raylib.h"
#include "GMath.hpp"

namespace gmath {

    class Vec2 {
    public:
        f32 x;
        f32 y;

        Vec2();
        explicit Vec2(f32 x, f32 y);
        explicit Vec2(f32 x, i32 y);
        explicit Vec2(i32 x, f32 y);
        explicit Vec2(i32 x, i32 y);

        // Arithmetic operators

        Vec2& operator+=(const Vec2& r);
        Vec2& operator-=(const Vec2& r);
        Vec2& operator*=(f32 scalar);
        Vec2& operator/=(f32 scalar);

        friend Vec2 operator+(const Vec2& l, const Vec2& r) {
            return Vec2 { l.x + r.x, l.y + r.y };
        }

        friend Vec2 operator-(const Vec2& l, const Vec2& r) {
            return Vec2 { l.x - r.x, l.y - r.y };
        }

        friend Vec2 operator*(const Vec2& l, const f32 scalar) {
            return Vec2 { l.x * scalar, l.y * scalar };
        }

        friend Vec2 operator*(const f32 scalar, const Vec2& r) {
            return Vec2 { r.x * scalar, r.y * scalar };
        }

        friend Vec2 operator/(const Vec2& l, const f32 scalar) {
            return Vec2 { l.x / scalar, l.y / scalar };
        }

        friend Vec2 operator/( const f32 scalar, const Vec2& r) {
            return Vec2 { r.x / scalar, r.y / scalar };
        }

        // Conversions
        Vector2 ToRaylib() {
            return Vector2 { x, y };
        }

        [[nodiscard]] Vec2 ToVec2Int() const {
            return Vec2 { static_cast<f32>(Round(x)), static_cast<f32>(Round(y)) };
        }

    };
}

#endif //GMATH_VEC2_HPP
