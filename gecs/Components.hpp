//
// Created by gaetz on 02/07/2023.
//

#ifndef GECS_COMPONENTS_HPP
#define GECS_COMPONENTS_HPP

#include <variant>
#include "Types.hpp"
#include "Asserts.hpp"
#include "Log.hpp"
#include <algorithm>

#include "Rect.hpp"

using gmath::Rect;
using gmath::Vec2;

#include "unordered_map"

using std::unordered_map;

namespace gecs {

    struct Column;

    /**
     * 2D position
     */
    struct Position {
        f32 x{0.0f};
        f32 y{0.0f};

        /**
         * Set the position with a Vec2
         * @param newVec Position to set
         */
        void Set(const Vec2 &newVec) {
            x = newVec.x;
            y = newVec.y;
        }

        /**
         * Return the position as a Vec2
         * @return Vec2 position data
         */
        [[nodiscard]] Vec2 Vec() const {
            return Vec2{x, y};
        }
    };

    /**
     * 2D velocity
     */
    struct Velocity {
        f32 x{0.0f};
        f32 y{0.0f};

        /**
         * Get the velocity as a Vec2
         * @return Vec2 velocity data
         */
        [[nodiscard]] Vec2 Vec() const {
            return Vec2{x, y};
        }
    };

    /**
     * 2D sprite with texture handle, source rect and destination size.
     * Destination rect will be set up using the position component.
     */
    struct Sprite {

        Sprite() = default;

        explicit Sprite(Texture textureP):
                texture{textureP},
                srcRect { 0.0f, 0.0f, static_cast<f32>(textureP.width), static_cast<f32>(textureP.height) },
                dstSize { srcRect.width, srcRect.height }
        {}

        Texture texture{};
        Rect srcRect;
        Vec2 dstSize{1.0f, 1.0f};
    };

    /**
     * Log a component's data
     * @param componentId Component type
     * @param column Column of the component in the archetype
     * @param row Row to log, corresponding to an entity
     * @return Displayable string
     */
    str LogComponent(ComponentId componentId, const Column &column, size_t row);

    class Entity;
}


#endif //GECS_COMPONENTS_HPP
