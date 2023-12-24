//
// Created by gaetz on 02/07/2023.
//

#ifndef GECSS_TYPES_HPP
#define GECSS_TYPES_HPP

#include <bitset>
using std::bitset;

#include <unordered_map>
using std::unordered_map;

#include "Defines.hpp"

namespace gecs {

    constexpr u32 MAX_COMPONENTS = 32;
    constexpr u64 LAST_32_BITS = 0b0000000000000000000000000000000011111111111111111111111111111111;

    using Id = u64;
    using ArchetypeId = bitset<MAX_COMPONENTS>;

    enum class ComponentId {
        Position = 0,
        Velocity = 1,
        Sprite = 2,
    };

    str ComponentIdToString(ComponentId componentId);
    ComponentId IndexToComponentId(i32 index);
    str ArchetypeIdToString(ArchetypeId id);
    u32 ComponentIdToBitValue(ComponentId componentId);


    class Archetype;

    struct ArchetypeRecord {
        /// Reference to entity's archetype
        Archetype* archetype;
        /// Entity row in archetypes component table
        u64 row;
    };

}

#endif //GECSS_TYPES_HPP
