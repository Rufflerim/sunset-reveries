//
// Created by gaetz on 02/07/2023.
//

#ifndef GECS_COMPONENTS_HPP
#define GECS_COMPONENTS_HPP

#include "Types.hpp"

#include "Vec2.hpp"
using gmath::Vec2;

#include "unordered_map"
using std::unordered_map;


enum class ComponentId {
    Position = 0,
    Velocity = 1
};

struct Position {
    f32 x { 0.0f };
    f32 y { 0.0f };

    Vec2 Vec() const {
        return Vec2 { x, y };
    }
};

struct Velocity {
    f32 x { 0.0f };
    f32 y { 0.0f };

    Vec2 Vec() const {
        return Vec2 { x, y };
    }
};

namespace gecs {

    class Entity;

    class Component {
    public:
        Id id;
    };
}


#endif //GECS_COMPONENTS_HPP
