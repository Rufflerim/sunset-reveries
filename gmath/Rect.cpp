//
// Created by gaetz on 13/06/2023.
//

#include "Rect.hpp"

namespace gmath {

    Rect::Rect() :
        x{0}, y{0}, width{1}, height{1}
    {}

    Rect::Rect(f32 xP, f32 yP, f32 w, f32 h) :
        x { xP }, y { yP }, width { w }, height { h }
    {}

    Rect::Rect(Vec2 pos, Vec2 size) :
        x { pos.x }, y { pos.y },
        width { size.x }, height { size.y }
    {}

    str Rect::Log() const {
        return "{ x: " + std::to_string(x) + ", y:" + std::to_string(y) +
                ", width: " + std::to_string(width) + ", height:" + std::to_string(height) + " } ";
    }

    Rectangle Rect::ToRaylib() const {
        return Rectangle{x, y, width, height};
    }

    Rect Rect::ToRectInt() const {
        return Rect {
            static_cast<f32>(Round(x)),
            static_cast<f32>(Round(y)),
            static_cast<f32>(Round(width)),
            static_cast<f32>(Round(height))
        };
    }

    Vec2 Rect::Position() const {
        return Vec2 { x, y };
    }

    Vec2 Rect::Size() const {
        return Vec2 { width, height };
    }

}