//
// Created by gaetz on 02/07/2023.
//

#ifndef GECS_COMPONENTS_HPP
#define GECS_COMPONENTS_HPP

#include <variant>
#include "Types.hpp"
#include "Asserts.hpp"

#include "Vec2.hpp"
using gmath::Vec2;

#include "unordered_map"
using std::unordered_map;


// Position

struct Position {
    f32 x { 0.0f };
    f32 y { 0.0f };

    Vec2 Vec() const {
        return Vec2 { x, y };
    }
};

/*
class PositionColumn : public IColumn {
public:
    gecs::ComponentId id { gecs::ComponentId::Position };
    vector<Position> data;

    void* GetDataRow(size_t row) override {
        return &data[row];
    };

    u64 AddElement(void* newData, size_t sizeOfData) override {
        GASSERT_MSG(sizeof(Position) == sizeOfData, "New element size must be size of component column type (Position)")
        auto* newDataPtr = static_cast<Position*>(newData);
        Position newElement;
        newElement.x = newDataPtr->x;
        newElement.y = newDataPtr->y;
        data.push_back(newElement);
        return data.size() - 1;
    }

    void RemoveElement(u64 row) override {
        data.erase(data.begin() + row);
    };
};
 */


// Velocity

struct Velocity {
    f32 x { 0.0f };
    f32 y { 0.0f };

    Vec2 Vec() const {
        return Vec2 { x, y };
    }
};

namespace gecs {

    template <class T>
    ComponentId ToComponentId() {
        if constexpr (std::is_same_v<T, Position>) {
            return ComponentId::Position;
        } else if constexpr (std::is_same_v<T, Velocity>) {
            return ComponentId::Velocity;
        }
    }

    class IColumn {
    public:
        template<class T>
        void Init(size_t numberOfElements = 100) {
            data.reserve(numberOfElements);
            dataSize = sizeof(T);
            componentId = ToComponentId<T>();
        }

        template<class T>
        T& GetRow(size_t row) {
            return std::get<T>(data[row]);
        }

        template<class T>
        const T& GetRowConst(size_t row) const {
            return std::get<T>(data[row]);
        }

        size_t GetDataMemorySize() const { return dataSize; }
        size_t Count() const  { return data.size(); }
        ComponentId GetComponentId() const { return componentId; }


        template<class T>
        u64 AddElement(T element) {
            data.push_back(std::move(element));
            return data.size() - 1;
        };

        void RemoveElement(u64 row) {
            data.erase(data.begin() + static_cast<i64>(row));
        };

        /* Functions used for logging purpose */

        const Position& GetPos(size_t row) const;
        const Velocity& GetVelocity(size_t row) const;

    private:
        vector<std::variant<Position, Velocity>> data;
        size_t dataSize;
        ComponentId componentId;
    };

    str LogComponent(ComponentId componentId, const IColumn& column, size_t row);

    class Entity;
}


#endif //GECS_COMPONENTS_HPP
