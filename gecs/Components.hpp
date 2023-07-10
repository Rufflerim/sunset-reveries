//
// Created by gaetz on 02/07/2023.
//

#ifndef GECS_COMPONENTS_HPP
#define GECS_COMPONENTS_HPP

#include "Types.hpp"
#include "Asserts.hpp"

#include "Vec2.hpp"
using gmath::Vec2;

#include "unordered_map"
using std::unordered_map;

class IColumn {
public:
    gecs::ComponentId id;

    virtual void* GetDataRow(size_t row) = 0;
    virtual size_t GetDataSize() = 0;
    virtual void AddElement(void* newData, size_t sizeOfData) = 0;
    virtual void RemoveElement(size_t row) = 0;
};


// Position

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

    void* GetDataRow(size_t row) override {
        return &data[row];
    };

    size_t GetDataSize() override {
        return sizeof(Position);
    };

    void AddElement(void* newData, size_t sizeOfData) override {
        GASSERT_MSG(sizeof(Position) == sizeOfData, "New element size must be size of component column type (Position)")
        auto* newDataPtr = static_cast<Position*>(newData);
        Position newElement;
        newElement.x = newDataPtr->x;
        newElement.y = newDataPtr->y;
        data.push_back(newElement);
    }

    void RemoveElement(size_t row) override {
        // Take row into account


    };
};


// Velocity

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

    void* GetDataRow(size_t row) override {
        return &data[row];
    };

    size_t GetDataSize() override {
        return sizeof(Velocity);
    };

    void AddElement(void* newData, size_t sizeOfData) override {
        GASSERT_MSG(sizeof(Velocity) == sizeOfData, "New element size must be size of component column type (Velocity)")
        auto* newDataPtr = static_cast<Velocity*>(newData);
        Velocity newElement;
        newElement.x = newDataPtr->x;
        newElement.y = newDataPtr->y;
        data.push_back(newElement);
    }

    void RemoveElement(size_t row) override {
        // Take row into account


    };
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
