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
        void SetCache(const std::tuple<vector<ComponentTypes>...> &newCache) {
            cache = newCache;
        }

        bool ShouldRefresh() const { return shouldRefresh; }

        std::tuple<vector<ComponentTypes>...> cache;
    private:
        bool shouldRefresh{false};
    };

    template<typename... ComponentTypes>
    class Query {
        friend QueryCache<ComponentTypes...>;

    public:
        void Each(std::function<void(ComponentTypes&...)> f) {
            auto &world = World::Instance();
            cache = world.Query<ComponentTypes...>();
            ApplyOnElements(f, std::make_index_sequence<sizeof...(ComponentTypes)>{});
        }


    private:
        std::tuple<std::vector<ComponentTypes>...> cache;

        template<typename FuncT, std::size_t... I>
        void ApplyOnElements(FuncT f, std::index_sequence<I...>) {
            auto minSize = std::min({std::get<I>(cache).size()...});
            for(std::size_t i = 0; i < minSize; ++i) {
                std::apply(f, std::tuple(std::ref(std::get<I>(cache)[i])...));
            }
            LOG(LogLevel::Warning) << "Query::ApplyOnElements: not implemented yet";
        }

    };
}
#endif //SUNSET_REVERIES_QUERY_HPP
