//
// Created by Gaëtan Blaise-Cazalet on 26/12/2023.
//

#ifndef GECS_QUERY_HPP
#define GECS_QUERY_HPP

#include <tuple>
#include <unordered_map>
#include "Defines.hpp"
#include "Types.hpp"
#include <functional>
#include "TupleUtils.hpp"
#include "QueryCache.hpp"
#include "QueryManager.hpp"

namespace gecs {

    /**
     * Queries are a way to gather and cache data from world,
     * in order to modify or read it in a more efficient way.
     * The right way to use them is to apply several modifications on
     * its components data then apply it to the world.
     *
     * @tparam ComponentTypes Component types to query
     */
    template<typename... ComponentTypes>
    class Query {
    public:
        /**
         * Create a new query and automatically compute the cache data
         */
        Query() {
            ResetCache();
        }

        /**
         * Apply a write function to all the entities' components of the query.
         * @param f Function to apply
         */
        void Update(std::function<void(ComponentTypes&...)> f) {
            cache.ApplyOnElements(f, true);
        }

        /**
         * Apply a read function to all the entities' components of the query.
         * Useful when you want to read data from the components without modifying them.
         * @param f Function to apply
         */
        void Read(std::function<void(const ComponentTypes&...)> f) {
            cache.ApplyOnElements(f, false);
        }

        /**
         * The passed function will check a condition on components and
         * delete the corresponding entity if it is true.
         * @param f Condition function, should return true if the entity should be deleted
         */
        void DeleteIf(std::function<bool(const ComponentTypes&...)> f) {
            if (cache.IsEmpty()) return;

            vector<Id> toDelete = cache.BuildFilteredEntityList(f, entities);
            if (toDelete.empty()) return;

            QueryManager::Instance().DestroyEntities(toDelete);
            Reset();
        }

        /**
         * The passed function will check a condition on components and
         * remove the component type passed as a template if it is true.
         * @tparam Component Component type to remove
         * @param f Condition function, should return true if the component should be removed
         */
        template<class Component>
        void RemoveIf(std::function<bool(const ComponentTypes&...)> f) {
            if (cache.IsEmpty()) return;

            vector<Id> removeCompEntities = cache.BuildFilteredEntityList(f, entities);
            if (removeCompEntities.empty()) return;

            // Apply so last modification (in AoS) is reintegrated in SoA and world
            Apply();

            auto& manager = QueryManager::Instance();
            for (const auto& toRemove : removeCompEntities) {
                manager.RemoveComponentFromEntity<Component>(toRemove);
            }

            Reset();
        }

        /**
         * Internally update the cache data then apply it to the world.
         * WARNING: This method is heavy and should be used with caution.
         */
        void Apply() {
            if (cache.IsEmpty()) return;
            const auto& lastCache = cache.RefreshAndReturnCache();
            QueryManager::Instance().ReintegrateQueryCache<ComponentTypes...>(lastCache);
            /// TODO Could also update the cache of other queries
        }

        /**
         * Reset the cache if it contains data for at least one entity.
         * It is useful to check this condition when entities are destroyed or
         * their components are removed, which imply an archetype change and might
         * make the query invalid.
         */
        void Reset() {
            if (cache.IsEmpty()) return;
            ResetCache();
        }


    private:
        /** Components data cache */
        QueryCache<ComponentTypes...> cache {};

        /** Entities ids, in the same order as the cache */
        vector<Id> entities {};

        /**
         * Compute the cache data and gather it from world,
         * also get the cache's entities ids in the same order.
         * WARNING: This method is heavy and should be used with caution.
         */
        void ResetCache() {
            auto result = QueryManager::Instance().ComputeQuery<ComponentTypes...>();
            entities = get<0>(result);
            cache = QueryCache<ComponentTypes...>(std::move(TupleTail(result)));
        }
    };
}
#endif //GECS_QUERY_HPP
