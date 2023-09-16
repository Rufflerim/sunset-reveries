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

        void Init();

        Id CreateEntity();

        Entity GetEntity(Id entityId);

        void LogWorld();

    private:

        u64 maxId;
        ArchetypeId defaultArchetype;

        template<class T>
        T& GetComponent(Id entity) {
            ComponentId componentId = ToComponentId<T>();
            ArchetypeRecord& record = entityRegistry[entity];
            Archetype* archetype = record.archetype;

            // Assert archetype has component
            ComponentArchetypes archetypes = componentRegistry[componentId];
            GASSERT_MSG(archetypes.count(archetype->archetypeId) != 0, "Archetype must have component to get it")

            size_t componentColumn = archetypes[archetype->archetypeId];
            return archetype->components[componentColumn].GetRow<T>(record.row);
        }

        u64 MoveEntity(const ArchetypeRecord& recordToUpdate, size_t row, Archetype* nextArchetype);

        // Singleton
    private:
        World() = default;

    public:
        static World& Instance() {
            static World instance;
            return instance;
        }

        World(World const&) = delete;

        void operator=(World const&) = delete;
    };

}


#endif //GECS_WORLD_HPP
