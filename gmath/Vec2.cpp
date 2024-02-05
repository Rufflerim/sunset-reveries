//
// Created by gaetz on 13/06/2023.
//

#include "Vec2.hpp"

namespace gmath {

    Vec2::Vec2() :
        x { 0 }, y { 0 }
    {}

    Vec2::Vec2(f32 xP, f32 yP) :
        x { xP }, y { yP }
    {}

    Vec2::Vec2(i32 xP, f32 yP) :
            x { static_cast<f32>(xP) }, y { yP }
    {}

    Vec2::Vec2(f32 xP, i32 yP) :
            x { xP }, y { static_cast<f32>(yP) }
    {}

    Vec2::Vec2(i32 xP, i32 yP) :
        x { static_cast<f32>(xP) }, y { static_cast<f32>(yP) }
    {}

    Vec2& Vec2::operator+=(const Vec2& r) {
        x += r.x;
        y += r.y;
        return *this;
    }

    Vec2& Vec2::operator-=(const Vec2& r) {
        x -= r.x;
        y -= r.y;
        return *this;
    }

    Vec2& Vec2::operator*=(f32 scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vec2& Vec2::operator/=(const f32 scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    str Vec2::Log() const {
        return "{ x: " + std::to_string(x) + ", y:" + std::to_string(y) + " } ";
    }
}
