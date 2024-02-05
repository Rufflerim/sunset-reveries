//
// Created by gaetz on 13/06/2023.
//

#ifndef GMATH_RECT_HPP
#define GMATH_RECT_HPP

#include "Vec2.hpp"

namespace gmath {
    class Rect {
    public:
        f32 x;
        f32 y;
        f32 width;
        f32 height;

        Rect();
        Rect(f32 x, f32 y, f32 w, f32 h);
        Rect(Vec2 pos, Vec2 size);
        str Log() const;

        [[nodiscard]] Vec2 Position() const;
        [[nodiscard]] Vec2 Size() const;

        [[nodiscard]] Rectangle ToRaylib() const;
        [[nodiscard]] Rect ToRectInt() const;
    };
}


#endif //GMATH_RECT_HPP
