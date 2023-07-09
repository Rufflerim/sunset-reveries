//
// Created by gaetz on 03/07/2023.
//

#ifndef GECS_WORLD_HPP
#define GECS_WORLD_HPP

#include "Types.hpp"
#include "Archetype.hpp"

namespace gecs {

    class World {
        friend class Entity;

    private:
        /**
         * Each entity has an a archetype reference
         */
        unordered_map<Id, ArchetypeRecord> entityRegistry;

        /**
         * Each archetype can be found through its list of components
         */
        unordered_map<ArchetypeId, Archetype> archetypeRegistry;

        /**
         * In which archetypes (and column inside archetype) we can find each component type
         */
        using ComponentArchetypes = unordered_map<ArchetypeId, size_t>;
        unordered_map<ComponentId, ComponentArchetypes> componentRegistry;

    public:
        unordered_map<Id, ArchetypeRecord>& GetEntities() { return entityRegistry; }
        unordered_map<ArchetypeId, Archetype>& GetArchetypes() { return archetypeRegistry; }
        unordered_map<ComponentId, ComponentArchetypes>& GetComponents() { return componentRegistry; }

    private:

        void MoveEntity(Id entity, Archetype* archetype, size_t row, Archetype* nextArchetype);


        template<class T>
        T& GetComponent(Id entity) {
            ComponentId componentId = GetComponentId<T>();
            ArchetypeRecord& record = entityRegistry[entity];
            Archetype* archetype = record.archetype;

            // Check if archetype has component
            ComponentArchetypes archetypes = componentRegistry[componentId];
            if (archetypes.count(archetype->archetypeId) == 0) {
                return nullptr;
            }

            size_t componentColumn = archetypes[archetype->archetypeId];
            return GetComponentEntityData<T>(archetype, componentColumn, record.row);
        }

        template <class T>
        T& GetComponentEntityData(Archetype* archetype, size_t componentColumn, size_t entityRow) {
            if constexpr (std::is_same_v<T, Position>) {
                auto col = static_cast<PositionColumn>(archetype->components[componentColumn]);
                return col.data[entityRow];
            } else if constexpr (std::is_same_v<T, Velocity>) {
                auto col = static_cast<VelocityColumn>(archetype->components[componentColumn]);
                return col.data[entityRow];
            }
        }

        // Singleton
    private:
        World() = default;
    public:
        static World& Instance() {
            static World instance;
            return instance;
        }

        World(World const&)          = delete;
        void operator=(World const&) = delete;
    };

}


#endif //GECS_WORLD_HPP
