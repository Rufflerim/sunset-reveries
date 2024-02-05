//
// Created by Gaëtan Blaise-Cazalet on 22/01/2024.
//

#include "Column.hpp"
#include "Components.hpp"

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
} // gecs