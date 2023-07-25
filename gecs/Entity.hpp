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
        explicit Entity(u64 idRoot);

        template<class T>
        void AddComponent(T componentData) {
            ComponentId component = ToComponentId<T>();
            auto& world = World::Instance();
            ArchetypeRecord& recordToUpdate = world.GetEntities()[id];
            Archetype* nextArchetype = recordToUpdate.archetype->archetypeChanges[component].add;
            // Add data in component column of new archetype
            u64 newRow;
            for (auto& column : nextArchetype->components) {
                if (component != column.id) continue;
                newRow = column.AddElement<T>(componentData);
                break;
            }
            // Move previous archetype data in new archetype
            if (recordToUpdate.archetype->archetypeId != 0) {
                world.MoveEntity<T>(recordToUpdate, recordToUpdate.row, nextArchetype);
            }

            // Update entity's row
            recordToUpdate.archetype = nextArchetype;
            recordToUpdate.row = newRow;
        }

        template<class T>
        void RemoveComponent() {
            ComponentId component = ToComponentId<T>();
            auto& world = World::Instance();
            ArchetypeRecord& recordToUpdate = world.GetEntities()[id];
            Archetype* nextArchetype = recordToUpdate.archetype->archetypeChanges[component].remove;
            // Move previous archetype data in new archetype
            u64 newRow = world.MoveEntity<T>(recordToUpdate, recordToUpdate.row, nextArchetype);
            // Update entity's row
            recordToUpdate.archetype = nextArchetype;
            recordToUpdate.row = newRow;
        }

        template<class T>
        T& GetComponent() {
            return World::Instance().GetComponent<T>(id);
        }
    };



}


#endif //GECS_ENTITY_HPP
