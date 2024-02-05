//
// Created by gaetz on 17/01/24.
//

#ifndef GECS_QUERYCACHE_HPP
#define GECS_QUERYCACHE_HPP

#include "Defines.hpp"
#include "Types.hpp"

namespace gecs {

    /**
     * Query cache actually stores two caches: one in Array of Structs (AoS) and one in Struct of Arrays (SoA).
     * The SoA cache is the way to have a link with the world's data, for this data is stored in archetypes,
     * containing a specific set of arrays of components.
     * The AoS cache is prepared to be read and updated, line by line, each line of it representing an entity's
     * components data.
     * @tparam ComponentTypes
     */
    template<typename... ComponentTypes>
    class QueryCache {
    public:
        QueryCache() = default;

        /**
         * Store the SoA cache and compute the AoS cache from it.
         * @param newCache
         */
        explicit QueryCache(const std::tuple<vector<ComponentTypes>...>&& newCache): cacheSoA{newCache} {
                size = std::get<0>(cacheSoA).size();
                cacheAoS.reserve(size);
                UpdateAoS();
        }

        /**
         * Apply a function on all elements of the AoS cache.
         * @tparam FuncT Function template, should take the same number of arguments as the number of components
         * @param f Function to apply
         * @param updating If true, the SoA cache will be marked as needing a refresh
         */
        template<typename FuncT>
        void ApplyOnElements(FuncT f, bool updating) {
            for(std::size_t i = 0; i < size; ++i) {
                std::apply(f, cacheAoS[i]);
            }
            shouldRefresh = updating;
        }

        /**
         * Build a list of entity ids (passed as a parameter) filtered by a function
         * that check conditions on the AoS cache data.
         * @tparam FuncT Function template, should take the same number of arguments as the number of components
         * and return a boolean
         * @param f The condition function
         * @param entities List of entity ids in the same order as the AoS cache
         * @return List of entity ids that passed the condition
         */
        template<typename FuncT>
        vector<Id> BuildFilteredEntityList(FuncT f, const vector<Id>& entities) {
            vector<Id> list;
            for(std::size_t i = 0; i < size; ++i) {
                if(std::apply(f, cacheAoS[i])) {
                    list.push_back(entities[i]);
                }
            }
            return list;
        }

        /**
         * Reset the SoA cache from the AoS cache, marking it as refreshed.
         */
        void RefreshCache() {
            if (shouldRefresh) {
                UpdateSoA();
                shouldRefresh = false;
            }
        }

        /**
         * Refresh the SoA cache from the AoS cache and return it for world updating purpose.
         * @return Refreshed SoA cache
         */
        const std::tuple<vector<ComponentTypes>...>& RefreshAndReturnCache() {
            RefreshCache();
            return cacheSoA;
        }

        /**
         * Check whether the AoS cache is empty.
         * @return True if empty
         */
        [[nodiscard]] bool IsEmpty() const { return cacheAoS.empty(); }

        /** Struct of Arrays cache, containing a vector for each component type, from different archetypes */
        std::tuple<vector<ComponentTypes>...> cacheSoA;

        /** Array of Structs cache, containing a tuple for each entity, with its components data */
        vector<std::tuple<ComponentTypes...>> cacheAoS;

    private:
        bool shouldRefresh{false};
        size_t size{0};

        /**
         * Transfer data from the SoA cache to the AoS cache.
         * The AoS cache is totally cleared and filled with new data.
         */
        void UpdateAoS() {
            cacheAoS.clear();
            for (size_t i = 0; i < size; ++i) {
                std::tuple<ComponentTypes...> tuple;
                std::apply([&](auto&&... comp) { ((comp = std::get<vector<std::decay_t<decltype(comp)>>>(cacheSoA)[i]), ...); }, tuple);
                cacheAoS.emplace_back(std::move(tuple));
            }
        }

        /**
         * Transfer data from the AoS cache to the SoA cache.
         * The size of the SoA cache is kept, but its vectors are cleared and filled with new data.
         */
        void UpdateSoA() {
            // Empty vectors but memory allocation is kept
            std::apply([](auto&&... vec) { (..., vec.clear()); }, cacheSoA);

            for (size_t i = 0; i < size; ++i) {
                std::apply([&](auto&&... vec) { (..., vec.push_back(std::get<ComponentTypes>(cacheAoS[i]))); }, cacheSoA);
            }
        }
    };

}

#endif //GECS_QUERYCACHE_HPP
