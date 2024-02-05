//
// Created by gaetz on 02/07/2023.
//

#include "Components.hpp"
#include "Column.hpp"

namespace gecs {

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
            default: {
                return "INVALID COMPONENT";
            }
        }
    }
}