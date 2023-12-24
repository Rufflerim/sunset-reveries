//
// Created by gaetz on 16/09/2023.
//

#ifndef GCAUSALITY_WORLDSTATE_HPP
#define GCAUSALITY_WORLDSTATE_HPP

#include "Types.hpp"
#include "Components.hpp"
#include "StringUtils.hpp"

using gecs::Id;
using gecs::ArchetypeId;
using gecs::Column;

namespace gcausality {

    struct ArchetypeData {
    public:
            Id id;
            ArchetypeId archetypeId;
            char name[12];
            vector<Column> components;

            ArchetypeData& operator= (const ArchetypeData& newArchetypeData) {
                id = newArchetypeData.id;
                archetypeId = newArchetypeData.archetypeId;
                gplatform::StrCpy(name, strlen(newArchetypeData.name), newArchetypeData.name);
                components = newArchetypeData.components;
                return *this;
            }
    };

    class WorldState {
    public:
        u64 currentFrame;
        vector<ArchetypeData> stateData;
    };
}
#endif //GCAUSALITY_WORLDSTATE_HPP
