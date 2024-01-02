//
// Created by Gaëtan Blaise-Cazalet on 26/12/2023.
//

#ifndef GECS_QUERY_HPP
#define GECS_QUERY_HPP

#include <tuple>
#include <unordered_map>
#include "Defines.hpp"
#include "Types.hpp"
#include "World.hpp"
#include <functional>

namespace gecs {

    template<typename... ComponentTypes>
    class QueryCache {
    public:
        QueryCache() = default;
        explicit QueryCache(std::tuple<vector<ComponentTypes>...> && newCache) : cacheSoA(std::move(newCache)) {
            size = std::get<0>(cacheSoA).size();
            cacheAoS.reserve(size);
            UpdateAoS();
            isEmpty = false;
        }

        template<typename FuncT>
        void ApplyOnElements(FuncT f) {
            for(std::size_t i = 0; i < size; ++i) {
                std::apply(f, cacheAoS[i]);
            }
            shouldRefresh = true;
        }

        void ReintegrateInWorld(World& world) {
            if (shouldRefresh) {
                UpdateSoA();
                shouldRefresh = false;
            }
            world.ReintegrateQueryCache<ComponentTypes...>(cacheSoA);
        }

        bool IsEmpty() const { return isEmpty; }

        std::tuple<vector<ComponentTypes>...> cacheSoA;
        vector<std::tuple<ComponentTypes...>> cacheAoS;
        
    private:
        bool isEmpty{true};
        bool shouldRefresh{false};
        size_t size{0};

        void UpdateAoS() {
            cacheAoS.clear();
            for (size_t i = 0; i < size; ++i) {
                std::tuple<ComponentTypes...> tuple;
                std::apply([&](auto&&... comp) { ((comp = std::get<vector<std::decay_t<decltype(comp)>>>(cacheSoA)[i]), ...); }, tuple);
                cacheAoS.emplace_back(std::move(tuple));
            }
        }

        void UpdateSoA() {
            // Empty vectors but memory allocation is kept
            std::apply([](auto&&... vec) { (..., vec.clear()); }, cacheSoA);

            for (size_t i = 0; i < size; ++i) {
                std::apply([&](auto&&... vec) { (..., vec.push_back(std::get<ComponentTypes>(cacheAoS[i]))); }, cacheSoA);
            }
        }
    };


    template<typename... ComponentTypes>
    class Query {
        friend QueryCache<ComponentTypes...>;

    public:
        void Each(std::function<void(ComponentTypes&...)> f) {
            auto &world = World::Instance();
            if (cache.IsEmpty()) {
                cache = QueryCache<ComponentTypes...>(world.Query<ComponentTypes...>());
            }
            cache.ApplyOnElements(f);
        }

        void Apply() {
            auto &world = World::Instance();
            cache.ReintegrateInWorld(world);
        }


    private:
        QueryCache<ComponentTypes...> cache;

        /*
        template<typename FuncT, std::size_t... I>
        void ApplyOnElements(FuncT f, std::index_sequence<I...>) {
            auto minSize = std::min({std::get<I>(cache).size()...});
            for(std::size_t i = 0; i < minSize; ++i) {
                std::apply(f, std::tuple(std::ref(std::get<I>(cache)[i])...));
            }
        }
        */
    };
}
#endif //GECS_QUERY_HPP
