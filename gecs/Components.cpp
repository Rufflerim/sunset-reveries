//
// Created by gaetz on 02/07/2023.
//

#include "Components.hpp"
#include "Entity.hpp"

namespace gecs {

    const Position& IColumn::GetPos(size_t row) const {
        return GetRowConst<Position>(row);
    }

    const Velocity& IColumn::GetVelocity(size_t row) const {
        return GetRowConst<Velocity>(row);
    }

    str LogComponent(ComponentId componentId, const IColumn& column, size_t row) {
        switch (componentId) {
            case ComponentId::Position: {
                auto& position = column.GetPos(row);
                return "{ x: " + std::to_string(position.x) + ", y:" + std::to_string(position.y) + " } ";
            }
            case ComponentId::Velocity: {
                auto& vel = column.GetVelocity(row);
                return "{ x: " + std::to_string(vel.x) + ", y:" + std::to_string(vel.y) + " } ";
            }
            case ComponentId::Absurd:
                return "";
        }
    }
}