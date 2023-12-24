//
// Created by gaetz on 03/07/2023.
//

#ifndef GECS_WORLD_HPP
#define GECS_WORLD_HPP

#include "Types.hpp"
#include "Archetype.hpp"
#include <tuple>

namespace gecs {

    struct CompArchIdAndCol {
        ComponentId componentId;
        ArchetypeId archId;
        size_t columnIndex;
    };

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
        Archetype* GetArchetype(const str& archetypeName);
        void LogWorld();

        template <typename... ComponentTypes>
        vector<CompArchIdAndCol> GetRelevantArchetypesAndCols() {
            vector<CompArchIdAndCol> ret;
            // Retrieve component ids from template
            vector<ComponentId> componentIds = ToComponentIds<ComponentTypes...>();
            // Create the minimal archetype id (bitset) we need
            // to find our archetypes and component cols
            std::bitset<32> pattern;
            for (const auto componentId : componentIds) {
                pattern.flip(static_cast<i32>(componentId));
            }
            // Check all archetypes by components to obtain component, archetypes and cols
            for (const auto& c : componentRegistry) {
                if (std::find(componentIds.begin(), componentIds.end(), c.first) == componentIds.end()) continue;
                ComponentArchetypes archs = c.second;
                for (const auto archIds : archs) {
                    // We check the archetype contain the minimal components
                    if ((archIds.first & pattern) == pattern) {
                        ret.push_back(CompArchIdAndCol { c.first, archIds.first, archIds.second });
                    }
                }
            }
            std::sort(ret.begin(), ret.end(), [=](CompArchIdAndCol a, CompArchIdAndCol b) {
                return a.componentId < b.componentId ||
                        (a.componentId == b.componentId && a.archId.to_ulong() < b.archId.to_ulong());
            });
            return ret;
        }

        template<typename T>
        vector<T> QuerySingleComp(const vector<CompArchIdAndCol>& filterMaterial) {
            ComponentId componentId = ToComponentId<T>();
            vector<CompArchIdAndCol> compFilter;
            for (auto compArchCol : filterMaterial) {
                if( compArchCol.componentId == componentId ) {
                    compFilter.emplace_back(compArchCol);
                }
            }

            vector<T> result;
            for (const auto& compArchCol : compFilter) {
                const ArchetypeId archId = compArchCol.archId;
                const size_t column = compArchCol.columnIndex;
                const auto count = archetypeRegistry[archId].components[column].Count();
                for (u32 i = 0; i < count; ++i) {
                    result.push_back(archetypeRegistry[archId].components[column].GetRow<T>(i));
                }
            }

            return result;
        }

        template<typename... ComponentTypes>
        std::tuple<std::vector<ComponentTypes>...> Query() {
            vector<CompArchIdAndCol> filterMaterial = GetRelevantArchetypesAndCols<ComponentTypes...>();
            return std::make_tuple(QuerySingleComp<ComponentTypes>(filterMaterial)...);
        }


    private:
        u64 maxId { 0 };
        ArchetypeId defaultArchetype;

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
