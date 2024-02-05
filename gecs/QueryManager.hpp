//
// Created by gaetz on 07/01/24.
//

#ifndef GECS_QUERYMANAGER_HPP
#define GECS_QUERYMANAGER_HPP

#include <ranges>
#include "Types.hpp"
#include "Components.hpp"
#include "Archetype.hpp"

namespace gecs {

    /**
     * The QueryManager is in charge of providing utilities for
     * the query class and stored queries when they are requested.
     *
     * It must not include the World class. That is why some member
     * functions just exist to get some data from world.
     */
    class QueryManager {
    public:
        /**
         * Destroys entities and related data which ids are passed
         * @param toDelete Entities' ids
         */
        static void DestroyEntities(const vector<Id>& toDelete);

        /**
         * We want to get all archetypes for the templated list of components,
         * and the columns in those archetypes that correspond to requested components.
         * @tparam ComponentTypes Requested components
         * @return Vector of CompArchIdAndCol, sorted by components then archetype id
         */
        template <typename... ComponentTypes>
        vector<CompArchIdAndCol> GetRelevantArchetypesAndCols() {
            vector<ComponentId> componentIds = ToComponentIds<ComponentTypes...>();
            // Create the minimal archetype id (bitset) we need
            // to find our archetypes and component cols
            ArchetypeId archetypeId = ToArchetypeId(componentIds);

            return GetComponentsWithArchsCols(std::move(componentIds), archetypeId);
        }

        template<typename T>
        vector<T> QuerySingleComp(const vector<CompArchIdAndCol>& filterMaterial) {

            ComponentId componentId = ToComponentId<T>();
            vector<CompArchIdAndCol> compFilter = FilterForOneComponent(filterMaterial, componentId);

            vector<T> result;
            for (const auto& compArchCol : compFilter) {
                const Column& column = GetColumn(compArchCol.archId, compArchCol.columnIndex);
                const size_t count = column.Count();
                for (u32 i = 0; i < count; ++i) {
                    result.push_back(column.GetRowConst<T>(i));
                }
            }

            return result;
        }

        /**
         * Look for requested component in all archetypes and organize the result
         * as a tuple of component's data vectors, with data sorted in component then
         * archetype id order. A vector of corresponding entities is also created.
         *
         * @tparam ComponentTypes Requested component combination (intersection)
         * @return Tuple with a vector of entities' id then vectors of entities' component data.
         */
        template<typename... ComponentTypes>
        std::tuple<vector<Id>, vector<ComponentTypes>...> ComputeQuery() {
            vector<CompArchIdAndCol> filterMaterial = GetRelevantArchetypesAndCols<ComponentTypes...>();
            if (filterMaterial.empty()) return {};

            // We want relevant entities in the same order as the query
            // -- Take all entities with their archetyp ids and rows
            vector<IdArchRow> entitiesWithArchRows = GetEntitiesWithArchsRows();

            // -- Get relevant archetypes
            vector<CompArchIdAndCol> compFilter = FilterForOneComponent(filterMaterial, filterMaterial[0].componentId);

            // Filter entities by archetype and then sort them by row
            auto filterLambda = [&compFilter](IdArchRow &a) {
                return std::find_if(compFilter.begin(), compFilter.end(), [&a](CompArchIdAndCol &filter) {
                    return filter.archId == a.archId;
                }) != compFilter.end();
            };
            auto sortLambda = [=](IdArchRow a, IdArchRow b) {
                return a.archId.to_ulong() < b.archId.to_ulong() ||
                       (a.archId.to_ulong() == b.archId.to_ulong() && a.row < b.row);
            };

            /// TODO Sort after filtering
            // -- Sort entities by archetype and row
            std::sort(entitiesWithArchRows.begin(), entitiesWithArchRows.end(), sortLambda);

            vector<Id> entities;
            for (const auto &entity: entitiesWithArchRows | std::views::filter(filterLambda)) {
                entities.push_back(entity.id);
            }

            // Get relevant component values in a tuple shape
            auto data = make_tuple(entities, QuerySingleComp<ComponentTypes>(filterMaterial)...);
            return data;
        }

        /**
         * Reintegrate query component data in world's archetypes.
         * It will compute in which archetype and column the data should be reintegrated,
         * and where in the query's data starts for a specific archetype's column.
         *
         * @tparam ComponentTypes Query's component types, that will be reintegrated
         * @param tuple Query's component data
         */
        template<typename... ComponentTypes>
        void ReintegrateQueryCache(const std::tuple<vector<ComponentTypes>...>& tuple) {
            vector<CompArchIdAndCol> compArchCols = GetRelevantArchetypesAndCols<ComponentTypes...>();
            // From compArchCols, Get vector of vector of pairs of archetypes id and cols, one vector for each component type
            auto archsAndCols = GetArchetypeAndColumnIndices(compArchCols);
            // Get start indices for each archetype containing the component type
            vector<vector<size_t>> starts = GetDataStartIndices(compArchCols);
            // Recursively call ReintegrateDataInColumn for each component type
            size_t i { 0 };
            size_t j { 0 };
            std::apply([&](auto&&... data) { (ReintegrateDataInColumn(std::move(data), starts[i++], archsAndCols[j++]), ...); }, tuple);
        }


        /**
         * Reintegrate one component type data in world's archetypes
         * @tparam T Component type
         * @param componentData Query's data for one component. Contains data for several archetypes.
         * @param start For each archetype, the index where the data starts in the query's data, in the same order as archsAndCols
         * @param archsAndCols Archetypes and columns where the data should be reintegrated, in the same order as start
         */
        template<typename T>
        void ReintegrateDataInColumn(const std::vector<T>&& componentData, const vector<size_t>& start, const vector<std::pair<ArchetypeId, size_t>>& archsAndCols) {
            unordered_map<ArchetypeId, Archetype>& archetypeRegistry = GetWorldArchetypes();
            for (u32 i = 0; i < start.size(); ++i) {
                const auto archId = archsAndCols[i].first;
                const auto column = archsAndCols[i].second;
                const auto count = archetypeRegistry[archId].components[column].Count();
                auto it = componentData.begin() + start[i];
                auto end = componentData.begin() + start[i] + count;
                vector<T> newData = std::vector<T>(it, end);
                archetypeRegistry[archId].components[column].ReplaceData<T>(std::move(newData));
            }
        }

        /**
         * Remove a component from an entity
         * @tparam T Component type
         * @param entityId Entity's id
         */
        template<typename T>
        void RemoveComponentFromEntity(Id entityId) {
            ComponentId componentId = ToComponentId<T>();
            RemoveComponentFromWorld(entityId, componentId);
        }

        /**
         * Store a query in the query manager
         * @param key Archetype id
         * @param q Query
         */
        void Store(ArchetypeId key, void* q);

        /**
         * Get a query from the query manager
         * @param key Archetype id
         * @return Query
         */
        void* Get(ArchetypeId key) {
            return queries[key];
        }

        /**
         * Check if a query has already been requested and stored
         * @param archId Archetype id
         * @return True if the query is stored
         */
        bool HasQuery(ArchetypeId archId);

    private:
        /** Map of stored queries */
        unordered_map<ArchetypeId, void*> queries;

        /** Utility, returns a reference to the world's archetypes */
        static unordered_map<ArchetypeId, Archetype>& GetWorldArchetypes();

        /**
         * Utility, returns entities with their archetypes and rows from world
         * @return
         */
        static vector<IdArchRow> GetEntitiesWithArchsRows();

        /**
         * Check all archetypes by components to obtain component, archetypes and cols
         * @param componentIds Relevant components
         * @param pattern Minimal pattern for archetypes
         * @return Vector of relevant component/archetype/column index data, ordered by component then archetype
         */
        static vector<CompArchIdAndCol> GetComponentsWithArchsCols(vector<ComponentId>&& componentIds, ArchetypeId pattern);

        /**
         * Utility, get a column from a world's archetype
         * @param archId Archetype id
         * @param column Column index
         * @return Column
         */
        static Column& GetColumn(ArchetypeId archId, size_t column);

        /**
         * Filter component/archetype/column data for a specific component
         * @param filterMaterial Data to filter
         * @param componentId Component id
         * @return Filtered data
         */
        static vector<CompArchIdAndCol> FilterForOneComponent(const vector<CompArchIdAndCol>& filterMaterial, ComponentId componentId);

        /**
         * From compArchCols, Get vector of vector of pairs of archetypes id and cols, one vector for each component type
         * @param compArchCols Vector of component ids with the component archetype/column index
         * @return A collection of one vector per component type, each vector containing pairs of
         * archetype id and column index, where components' data can be found
         */
        static vector<vector<std::pair<ArchetypeId, size_t>>> GetArchetypeAndColumnIndices(const vector<CompArchIdAndCol> &compArchCols);

        /**
         * When we want to reintegrate query's component data, we must find where the data for a specific archetype
         * is located. This function returns the start index for each archetype, grouped by component. It is sorted in
         * an order that matches the GetArchetypeAndColumnIndices's function result.
         * @param compArchCols Vector of component ids with the component archetype/column index
         * @return For each component, each vector gives indices where the data starts for each archetype, in the query's data
         */
        static vector<vector<size_t>> GetDataStartIndices(const vector<CompArchIdAndCol> &compArchCols);

        /**
         * Calls world's function to remove a component from an entity
         * @param entityId Entity's id we want a component removed from
         * @param componentId Component's id to remove
         */
        static void RemoveComponentFromWorld(Id entityId, ComponentId componentId);


        // Singleton
    private:
        QueryManager() = default;

    public:
        static QueryManager& Instance() {
            static QueryManager instance;
            return instance;
        }

        QueryManager(QueryManager const&) = delete;
        void operator=(QueryManager const&) = delete;
    };
}
#endif //GECS_QUERYMANAGER_HPP
