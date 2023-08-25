//
// Created by gaetz on 02/07/2023.
//

#include "Components.hpp"
#include "Entity.hpp"

namespace gecs {

    const Position& Column::GetPos(size_t row) const {
        return GetRowConst<Position>(row);
    }

    const Velocity& Column::GetVelocity(size_t row) const {
        return GetRowConst<Velocity>(row);
    }

    const Sprite &Column::GetSprite(size_t row) const {
        return GetRowConst<Sprite>(row);
    }

    str LogComponent(ComponentId componentId, const Column& column, size_t row) {
        switch (componentId) {
            case ComponentId::Position: {
                auto& position = column.GetPos(row);
                return position.Vec().Log();
            }
            case ComponentId::Velocity: {
                auto& vel = column.GetVelocity(row);
                return vel.Vec().Log();
            }
            case ComponentId::Sprite: {
                auto& sprite = column.GetSprite(row);
                return "{ src: " + sprite.srcRect.Log() + ", dst: " + sprite.dstSize.Log() + " } ";
            }
            case ComponentId::Absurd:
                return "";
        }
    }
}