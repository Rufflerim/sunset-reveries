//
// Created by gaetz on 03/07/2023.
//

#ifndef GECS_WORLD_HPP
#define GECS_WORLD_HPP

#include "Types.hpp"
#include "Archetype.hpp"
#include <tuple>
#include <ranges>
#include "Query.hpp"

namespace gecs {

    /**
     * World is an archetypal ECS pattern.
     * It stores entities in archetypes, which are collection of component columns.
     * Each entity has a reference to its archetype and its row in the archetype.
     * A component registry allow for quick lookup for a component type in archetypes.
     *
     * Switching from an archetype to an other is done by moving in a component graph. For now the graph
     * is manually created in the Init() function.
     */
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
        [[nodiscard]] const unordered_map<ArchetypeId, Archetype>& GetArchetypesConst() const { return archetypeRegistry; }
        unordered_map<ComponentId, ComponentArchetypes>& GetComponents() { return componentRegistry; }


        /**
         * Initialize world with default archetype graph
         */
        void Init();

        /**
         * Create an entity with empty archetype
         * @return Id of the new entity
         */
        Id CreateEntity();

        /**
         * Destroy an entity
         * @param entityId Id of the entity to destroy
         */
        void DestroyEntity(Id entityId);

        /**
         * Return an Entity object set with its id
         * @param entityId Id of the entity to get
         * @return Entity object
         */
        static Entity GetEntity(Id entityId);

        /**
         * Archetypes have a name for easy use. Return the archetype with the given name.
         * @param archetypeName Name of the archetype to get
         * @return Archetype pointer
         */
        Archetype* GetArchetype(const str& archetypeName);

        /**
         * World log for debug purpose.
         * Log all archetypes and their components.
         */
        void LogWorld();

        /**
         * Generate a query that contains vectors of specified components looked up in each archetype table.
         * If this query has already been generated, return the existing one.
         * @tparam ComponentTypes Components to look up
         * @return Query object
         */
        template<typename ...ComponentTypes>
        Query<ComponentTypes...>& Find() {
            QueryManager& queryManager = QueryManager::Instance();
            ArchetypeId archId = ToArchetypeId<ComponentTypes...>();
            if (!queryManager.HasQuery(archId)) {
                // Query is stored with no type for simplicity purpose.
                // However, this is the only place where it has no type.
                // It will be casted to Query<ComponentTypes...> when retrieved.
                void* query = new Query<ComponentTypes...>();
                queryManager.Store(archId, query);
            }
            auto ret = reinterpret_cast<Query<ComponentTypes...>*>(queryManager.Get(archId));
            return *ret;
        }

        /**
         * Add a component to an entity.
         * @tparam T Type of the component to add
         * @param id Id of the entity to add the component to
         * @param componentData Data of the component to add
         * For cache usage optimization, prefer small component types.
         */
        template<class T>
        void AddComponent(Id id, T componentData) {
            ComponentId component = ToComponentId<T>();
            ArchetypeRecord& recordToUpdate = GetEntities()[id];
            Archetype* nextArchetype = recordToUpdate.archetype->archetypeChanges[component].add;

            // Add data in component column of new archetype
            u64 newRow;
            for (auto& column : nextArchetype->components) {
                if (component != column.GetComponentId()) continue;
                newRow = column.AddElement<T>(componentData);
                break;
            }

            // Move previous archetype data in new archetype
            if (recordToUpdate.archetype->archetypeId != 0) {
                MoveEntity(recordToUpdate, nextArchetype);
            }

            // Update entity's row
            recordToUpdate.archetype = nextArchetype;
            recordToUpdate.row = newRow;
        }

        /**
         * Remove a component from an entity.
         * @tparam T Type of the component to remove
         * @param entityId Id of the entity to remove the component from
         */
        void RemoveComponent(Id entityId, ComponentId componentId);

        /**
         * Get a component from an entity.
         * @tparam T Type of the component to get
         * @param entity Id of the entity to get the component from
         * @return Reference to the component
         * This is clearly not the best way to work with components.
         * Prefer the usage of queries when you need to read or modify several components of several entities.
         */
        template<class T>
        T& GetComponent(Id entity) {
            const ComponentId componentId = ToComponentId<T>();
            const ArchetypeRecord& record = entityRegistry[entity];
            Archetype* archetype = record.archetype;

            // Assert archetype has component
            ComponentArchetypes archetypes = componentRegistry[componentId];
            GASSERT_MSG(archetypes.count(archetype->archetypeId) != 0, "Archetype must have component to get it")

            const size_t componentColumn = archetypes[archetype->archetypeId];
            return archetype->components[componentColumn].GetRow<T>(record.row);
        }

    private:
        /**
         * Max entity id.
         * TODO: Use a pool allocator for entities and components.
         */
        u64 maxId { 0 };

        /**
         * Default archetype for entities with no component.
         * Serve as a starting point for archetype graph.
         */
        ArchetypeId defaultArchetype;

        /**
         * Move an entity's data from one archetype to another. The entity to move is figured by its archetype record.
         * @param recordToUpdate
         * @param nextArchetype
         * @param remove
         * @return
         */
        u64 MoveEntity(const ArchetypeRecord& recordToUpdate, Archetype* nextArchetype, bool remove = false);

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
