//
// Created by gaetz on 03/07/2023.
//

#include "World.hpp"

namespace gecs {

    void World::Init() {
        // Init archetype ids with bits
        ArchetypeId emptyArchetypeId;
        ArchetypeId positionArchetypeId;
        positionArchetypeId.flip(0);
        ArchetypeId velocityArchetypeId;
        velocityArchetypeId.flip(1);
        ArchetypeId posVelArchetypeId;
        posVelArchetypeId.flip(0).flip(1);

        // Init archetypes
        Archetype emptyArchetype;
        emptyArchetype.id = 0;
        emptyArchetype.archetypeId = emptyArchetypeId;
        archetypeRegistry.insert(std::make_pair(emptyArchetypeId, std::move(emptyArchetype)));

        Archetype positionArchetype;
        positionArchetype.id = 1;
        positionArchetype.archetypeId = positionArchetypeId;

        /// TODO Add colums of components?
        PositionColumn col;
        positionArchetype.components.push_back(col);

        archetypeRegistry.insert(std::make_pair(positionArchetypeId, std::move(positionArchetype)));


        Archetype velocityArchetype;
        velocityArchetype.id = 2;
        velocityArchetype.archetypeId = velocityArchetypeId;
        archetypeRegistry.insert(std::make_pair(velocityArchetypeId, std::move(velocityArchetype)));

        Archetype posVelArchetype;
        posVelArchetype.id = 2;
        posVelArchetype.archetypeId = posVelArchetypeId;
        archetypeRegistry.insert(std::make_pair(posVelArchetypeId, std::move(posVelArchetype)));

        // Init archetype graph
        // -- Empty
        archetypeRegistry[emptyArchetypeId].archetypeChanges[ComponentId::Position].add = &archetypeRegistry[positionArchetypeId];
        archetypeRegistry[emptyArchetypeId].archetypeChanges[ComponentId::Position].remove = nullptr;
        archetypeRegistry[emptyArchetypeId].archetypeChanges[ComponentId::Velocity].add = &archetypeRegistry[velocityArchetypeId];
        archetypeRegistry[emptyArchetypeId].archetypeChanges[ComponentId::Velocity].remove = nullptr;
        // -- Position
        archetypeRegistry[positionArchetypeId].archetypeChanges[ComponentId::Position].add = &archetypeRegistry[positionArchetypeId];
        archetypeRegistry[positionArchetypeId].archetypeChanges[ComponentId::Position].remove = &archetypeRegistry[emptyArchetypeId];
        archetypeRegistry[positionArchetypeId].archetypeChanges[ComponentId::Velocity].add = &archetypeRegistry[posVelArchetypeId];
        archetypeRegistry[positionArchetypeId].archetypeChanges[ComponentId::Velocity].remove = nullptr;
        // -- Velocity
        archetypeRegistry[velocityArchetypeId].archetypeChanges[ComponentId::Position].add = &archetypeRegistry[posVelArchetypeId];
        archetypeRegistry[velocityArchetypeId].archetypeChanges[ComponentId::Position].remove = nullptr;
        archetypeRegistry[velocityArchetypeId].archetypeChanges[ComponentId::Velocity].add = &archetypeRegistry[velocityArchetypeId];
        archetypeRegistry[velocityArchetypeId].archetypeChanges[ComponentId::Velocity].remove = &archetypeRegistry[emptyArchetypeId];
        // -- Position and Velocity
        archetypeRegistry[posVelArchetypeId].archetypeChanges[ComponentId::Position].add = &archetypeRegistry[posVelArchetypeId];
        archetypeRegistry[posVelArchetypeId].archetypeChanges[ComponentId::Position].remove = &archetypeRegistry[velocityArchetypeId];
        archetypeRegistry[posVelArchetypeId].archetypeChanges[ComponentId::Velocity].add = &archetypeRegistry[posVelArchetypeId];
        archetypeRegistry[posVelArchetypeId].archetypeChanges[ComponentId::Velocity].remove = &archetypeRegistry[positionArchetypeId];
    }

    u64 World::MoveEntity(const ArchetypeRecord& recordToUpdate, size_t row, Archetype* nextArchetype) {
        u64 newRow;
        // Insert in new archetype data from previous archetype
        for (IColumn& dstCol : nextArchetype->components) {
            for (IColumn& srcCol : recordToUpdate.archetype->components) {
                if (dstCol.id != srcCol.id) continue;

                // Copy data in new component columns
                void* data = srcCol.GetDataRow(row);
                newRow = dstCol.AddElement(data, srcCol.GetDataSize());
                // Remove previous data from archetype, after saving data
                srcCol.RemoveElement(row);
            }
        }
        return newRow;
    }


}