//
// Created by gaetz on 02/07/2023.
//

#ifndef GECSS_TYPES_HPP
#define GECSS_TYPES_HPP

#include <bitset>
using std::bitset;

#include "Defines.hpp"

namespace gecs {

    constexpr u32 MAX_COMPONENTS = 32;
    constexpr u64 LAST_32_BITS = 0b0000000000000000000000000000000011111111111111111111111111111111;

    using Id = u64;
    using ArchetypeId = bitset<MAX_COMPONENTS>;



}

#endif //GECSS_TYPES_HPP
