//
// Created by gaetz on 16/09/2023.
//

#include "Defines.hpp"

#include "Archetype.hpp"
#include "WorldState.hpp"

using gecs::Archetype;

#ifndef GCAUSALITY_LAW_HPP
#define GCAUSALITY_LAW_HPP

namespace gcausality {

    class Law {
    public:
        Law(const str& archetypeName, i32 order);

        [[nodiscard]] i32 GetOrder() const { return order; }
        ArchetypeData Apply();

    private:
        i32 order;
        str archetypeName;
        Archetype* archetype { nullptr };
        std::function<ArchetypeData(Archetype*)> rule { nullptr };
    };

}

#endif //GCAUSALITY_LAW_HPP
