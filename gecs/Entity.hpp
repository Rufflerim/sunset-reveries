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
        Id id;

        template<class T>
        void AddComponent() {
            ComponentId component = GetComponentId<T>();
            auto& world = World::Instance();
            ArchetypeRecord& record = world.GetEntities()[id];
            Archetype* archetype = record.archetype;
            Archetype* nextArchetype = archetype->archetypeChanges[component].add;
            world.MoveEntity(id, archetype, record.row, nextArchetype);
        }

        template<class T>
        T& GetComponent() {
            return World::Instance().GetComponent<T>(id);
        }
    };



}


#endif //GECS_ENTITY_HPP
