//
// Created by gaetz on 03/07/2023.
//

#ifndef GECS_WORLD_HPP
#define GECS_WORLD_HPP

#include "Types.hpp"
#include "Archetype.hpp"
#include <tuple>
#include <ranges>

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

        void Init();
        Id CreateEntity();
        void DestroyEntity(Id entityId);
        Entity GetEntity(Id entityId);
        Archetype* GetArchetype(const str& archetypeName);
        void LogWorld();

        struct IdArchRow {
            Id id;
            ArchetypeId archId;
            size_t row;
        };

        template<typename... ComponentTypes>
        std::tuple<vector<Id>, vector<ComponentTypes>...> Query() {
            vector<CompArchIdAndCol> filterMaterial = GetRelevantArchetypesAndCols<ComponentTypes...>();
            // We want relevant entities in the same order as the query

            // Take all entities, filter them by archetype and then sort them by row
            vector<IdArchRow> entitiesWithArchRows;
            for (const auto& entity : entityRegistry) {
                entitiesWithArchRows.push_back(IdArchRow { entity.first, entity.second.archetype->archetypeId, entity.second.row });
            }

            // Get relevant archetypes
            ComponentId componentId = filterMaterial[0].componentId;
            vector<CompArchIdAndCol> compFilter;
            for (auto compArchCol : filterMaterial) {
                if( compArchCol.componentId == componentId ) {
                    compFilter.emplace_back(compArchCol);
                }
            }

            auto filterLambda = [&compFilter](IdArchRow& a){
                return std::find_if(compFilter.begin(), compFilter.end(), [&a](CompArchIdAndCol& filter){
                    return filter.archId == a.archId;
                }) != compFilter.end();
            };
            auto sortLambda = [=](IdArchRow a, IdArchRow b) {
                return a.archId.to_ulong() < b.archId.to_ulong() ||
                       (a.archId.to_ulong() == b.archId.to_ulong() && a.row < b.row);
            };

            // Filter entities by archetype existing in compFilter
         //   entitiesWithArchRows = entitiesWithArchRows | std::views::filter();

            // Sort entities by archetype and row
            std::sort(entitiesWithArchRows.begin(), entitiesWithArchRows.end(), sortLambda);

            vector<Id> entities;
            for (const auto& entity : entitiesWithArchRows | std::views::filter(filterLambda)) {
                entities.push_back(entity.id);
            }

            // Get relevant component values in a tuple shape
            return make_tuple(entities, QuerySingleComp<ComponentTypes>(filterMaterial)...);
        }

        template<typename... ComponentTypes>
        void ReintegrateQueryCache(std::tuple<vector<ComponentTypes>...> tuple) {
            vector<CompArchIdAndCol> compArchCols = GetRelevantArchetypesAndCols<ComponentTypes...>();
            // From compArchCols, Get vector of vector of pairs of archetypes id and cols,one vector for each component type
            auto archsAndCols = GetArchetypeAndColumnIndices(compArchCols);
            // Get start indices for each archetype containing the component type
            vector<vector<size_t>> starts = GetDataStartIndices(compArchCols);
            // Recursively call ReintegrateDataInColumn for each component type
            size_t i { 0 };
            size_t j { 0 };
            std::apply([&](auto&&... data) { (ReintegrateDataInColumn(data, starts[i++], archsAndCols[j++]), ...); }, tuple);
        }

        template<typename T>
        void ReintegrateDataInColumn(const std::vector<T>& tupleData, vector<size_t> start, vector<std::pair<ArchetypeId, size_t>> archsAndCols) {
            for (u32 i = 0; i < start.size(); ++i) {
                const auto archId = archsAndCols[i].first;
                const auto column = archsAndCols[i].second;
                const auto count = archetypeRegistry[archId].components[column].Count();
                auto it = tupleData.begin() + start[i];
                auto end = tupleData.begin() + start[i] + count;
                vector<T> newData = std::vector<T>(it, end);
                archetypeRegistry[archId].components[column].ReplaceData<T>(newData);
            }
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
                    // We also exclude empty archetypes
                    if ((archIds.first & pattern) == pattern && archetypeRegistry[archIds.first].GetRowCount() > 0) {
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
                    result.push_back(archetypeRegistry[archId].components[column].GetRowConst<T>(i));
                }
            }

            return result;
        }

        u64 MoveEntity(const ArchetypeRecord& recordToUpdate, size_t row, Archetype* nextArchetype);

        vector<vector<std::pair<ArchetypeId, size_t>>> GetArchetypeAndColumnIndices(vector<CompArchIdAndCol> &compArchCols);

        vector<vector<size_t>> GetDataStartIndices(vector<CompArchIdAndCol> &compArchCols);

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
