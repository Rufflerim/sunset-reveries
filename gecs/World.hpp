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

        template <class T>
        u64 MoveEntity(const ArchetypeRecord& recordToUpdate, size_t row, Archetype* nextArchetype) {
            u64 checkRow = std::numeric_limits<uint64_t>::max();
            u64 newRow = std::numeric_limits<uint64_t>::max();
            i32 checkColsDst { 0 }; // Used to avoid empty archetype case
            i32 checkColsSrc { 0 }; // Used to avoid empty archetype case

            // Insert in new archetype data from previous archetype
            for (IColumn& dstCol : nextArchetype->components) {
                for (IColumn& srcCol : recordToUpdate.archetype->components) {
                    if (dstCol.GetComponentId() != srcCol.GetComponentId()) continue;

                    // Copy data in new component columns
                    auto component = srcCol.GetRow<T>(row);
                    newRow = dstCol.AddElement<T>(component);
                    // Check row is the same for each column
                    if (checkRow != std::numeric_limits<uint64_t>::max()) {
                        GASSERT_MSG(checkRow == newRow, "Row must be the same in all archetype columns");
                    }
                    checkRow = newRow;

                    /*
                    void* data = srcCol.GetDataRow(row);
                    newRow = dstCol.AddElement(data, srcCol.GetDataMemorySize());
                    */

                    // Remove previous data from archetype, after saving data
                    srcCol.RemoveElement(row);

                    ++checkColsSrc;
                }
                ++checkColsDst;
            }
            GASSERT_MSG(newRow != std::numeric_limits<uint64_t>::max() && checkColsDst > 0 && checkColsSrc > 0, "Row should exist");
            return newRow;
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
