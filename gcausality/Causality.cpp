//
// Created by gaetz on 16/09/2023.
//

#include "Causality.hpp"
#include "World.hpp"

#include <algorithm>

using gecs::World;

namespace gcausality {

    void Causality::AddLaw(const str& archetypeName, i32 order) {
        laws.emplace_back(archetypeName, order);
        std::sort(std::begin(laws), std::end(laws),
                  [](const Law& lawA, const Law& lawB) { return lawA.GetOrder() < lawB.GetOrder(); });
    }

    WorldState Causality::Tick(f32 dt) {
        vector<ArchetypeData> newWorldData;
        for (auto& law : laws) {
            // TODO how to handle multiple laws for same archetype ?
            // TODO how to handle entity destruction
            // TODO how to handle archetype change (component addition and removal)
            newWorldData.push_back(law.Apply());
        }

        // Copy current world data
        // TODO, could use previous stored data
        auto& currentWorld = World::Instance().GetArchetypes();
        WorldState futureWorldState;
        futureWorldState.currentFrame = stateManager.GetCurrentFrame();
        for (auto& archetypePair : currentWorld) {
            auto& archetype = archetypePair.second;
            ArchetypeData archetypeData;
            archetypeData.id = archetype.id;
            archetypeData.archetypeId = archetype.archetypeId;
            strcpy_s(archetypeData.name, strnlen_s(archetype.name, 12), archetype.name);
            archetypeData.components = archetype.components;
            futureWorldState.stateData.push_back(archetypeData);
        }

        // Put back consequences of laws on world data
        // TODO System of filters to avoid applying some laws
        for (auto& modifiedArchetypeData : newWorldData) {
            for (auto& archetypeData : futureWorldState.stateData) {
                if (archetypeData.archetypeId == modifiedArchetypeData.archetypeId) {
                    archetypeData = modifiedArchetypeData;
                }
            }
        }

        return futureWorldState;
    }
}