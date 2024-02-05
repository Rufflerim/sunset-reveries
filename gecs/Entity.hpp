//
// Created by gaetz on 02/07/2023.
//

#ifndef GECS_ENTITY_HPP
#define GECS_ENTITY_HPP

#include "Defines.hpp"
#include "Types.hpp"
#include "World.hpp"

namespace gecs {

    /**
     * Represents a game entity.
     */
    class Entity {

    public:
        /**
         * Entity id
         */
        Id id;

        /**
         * Create an entity with a specific id.
         * @param idP Entity id
         */
        explicit Entity(u64 idP);

        /**
         * Shortcut to get an entity's component.
         * Useful only when targeting a specific entity. For multiple entities, use a query.
         * @tparam T Component type
         * @return Reference to the component
         */
        template<typename T>
        T& GetComponent() {
            return World::Instance().GetComponent<T>(id);
        }
    };



}


#endif //GECS_ENTITY_HPP
