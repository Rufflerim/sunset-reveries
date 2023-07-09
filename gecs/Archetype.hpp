//
// Created by gaetz on 02/07/2023.
//

#ifndef GECS_ARCHETYPE_HPP
#define GECS_ARCHETYPE_HPP

#include "Types.hpp"
#include "Components.hpp"

namespace gecs {

    class Archetype;

    struct ArchetypeChange {
        Archetype* add;
        Archetype* remove;
    };

    class Archetype {
    public:
        Id id;
        ArchetypeId archetypeId;
        vector<IColumn> components;
        unordered_map<ComponentId, ArchetypeChange> archetypeChanges;



    };



}


#endif //SUNSET_REVERIES_ARCHETYPE_HPP
