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
#include "GMath.hpp"

namespace gecs {

    constexpr u32 MAX_COMPONENTS = 32;
    constexpr u64 LAST_32_BITS = 0b0000000000000000000000000000000011111111111111111111111111111111;

    using Id = u64;
    using ArchetypeId = bitset<MAX_COMPONENTS>;

    enum class ComponentId {
        Position = 0,
        Velocity = 1
    };

    u32 ComponentIdToBitValue(ComponentId componentId) {
        f32 id = static_cast<f32>(componentId);
        return gmath::Floor(gmath::Pow(2, id));
    }



    struct Column {
    public:
        ComponentId componentId;
        size_t elementSize;
        size_t count;
        void* elements;
    };

    class Archetype;

    struct ArchetypeRecord {
        /// Reference to entity's archetype
        Archetype* archetype;
        /// Entity row in archetypes component table
        size_t row;
    };

}

#endif //GECSS_TYPES_HPP
