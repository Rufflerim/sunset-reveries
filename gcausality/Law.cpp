//
// Created by gaetz on 16/09/2023.
//

#include "Law.hpp"
#include "Asserts.hpp"
#include "World.hpp"

using gecs::World;

namespace gcausality {

    Law::Law(const str& archetypeName, i32 order) :
            order { order },
            archetype { World::Instance().GetArchetype(archetypeName) } {
        GASSERT_MSG(archetype != nullptr, "Could not find archetype " + archetypeName);
    }

    ArchetypeData Law::Apply() {
        return rule(archetype);
    }

}