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

class IColumn {
public:
    gecs::ComponentId id;
};

struct Position {
    f32 x { 0.0f };
    f32 y { 0.0f };

    Vec2 Vec() const {
        return Vec2 { x, y };
    }
};

class PositionColumn : public IColumn {
public:
    gecs::ComponentId id { gecs::ComponentId::Position };
    vector<Position> data;
};

struct Velocity {
    f32 x { 0.0f };
    f32 y { 0.0f };

    Vec2 Vec() const {
        return Vec2 { x, y };
    }
};

class VelocityColumn : public IColumn {
public:
    gecs::ComponentId id { gecs::ComponentId::Velocity };
    vector<Velocity> data;
};

namespace gecs {

    class Entity;

    class Component {
    public:
        Id id;
    };

    template <class T>
    ComponentId GetComponentId() {
        if constexpr (std::is_same_v<T, Position>) {
            return ComponentId::Position;
        } else if constexpr (std::is_same_v<T, Velocity>) {
            return ComponentId::Velocity;
        }
    }
}


#endif //GECS_COMPONENTS_HPP
