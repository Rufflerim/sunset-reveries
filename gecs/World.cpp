//
// Created by gaetz on 03/07/2023.
//

#include "World.hpp"
#include "Entity.hpp"

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
        defaultArchetype = emptyArchetypeId;

        // Position archetype
        Archetype positionArchetype;
        positionArchetype.id = 1;
        positionArchetype.archetypeId = positionArchetypeId;
        IColumn posArchCol;
        posArchCol.Init<Position>(100);
        positionArchetype.components.push_back(std::move(posArchCol));
        archetypeRegistry.insert(std::make_pair(positionArchetypeId, std::move(positionArchetype)));

        ComponentArchetypes componentArchetypePos;
        componentArchetypePos.insert(std::make_pair(positionArchetypeId, 0));
        componentRegistry.insert(std::make_pair(ComponentId::Position, componentArchetypePos));

        // Velocity archetype
        Archetype velocityArchetype;
        velocityArchetype.id = 2;
        velocityArchetype.archetypeId = velocityArchetypeId;
        IColumn velArchCol;
        velArchCol.Init<Velocity>(100);
        velocityArchetype.components.push_back(std::move(velArchCol));
        archetypeRegistry.insert(std::make_pair(velocityArchetypeId, std::move(velocityArchetype)));

        ComponentArchetypes componentArchetypeVel;
        componentArchetypeVel.insert(std::make_pair(velocityArchetypeId, 0));
        componentRegistry.insert(std::make_pair(ComponentId::Velocity, componentArchetypeVel));


        // Pos Vel Archetype
        Archetype posVelArchetype;
        posVelArchetype.id = 3;
        posVelArchetype.archetypeId = posVelArchetypeId;
        IColumn posVelArchPosCol;
        posVelArchPosCol.Init<Position>(100);
        posVelArchetype.components.push_back(std::move(posVelArchPosCol));
        IColumn posVelArchVelCol;
        posVelArchVelCol.Init<Velocity>(100);
        posVelArchetype.components.push_back(std::move(posVelArchVelCol));
        archetypeRegistry.insert(std::make_pair(posVelArchetypeId, std::move(posVelArchetype)));

        ComponentArchetypes componentArchetypePosVel;
        componentArchetypePosVel.insert(std::make_pair(posVelArchetypeId, 0));
        componentRegistry[ComponentId::Position].insert(std::make_pair(posVelArchetypeId, 0));
        componentRegistry[ComponentId::Velocity].insert(std::make_pair(posVelArchetypeId, 1));


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

    Id World::CreateEntity() {
        auto entity = Entity(maxId++);
        ArchetypeRecord newEntityRecord { &archetypeRegistry[defaultArchetype], entityRegistry.size() - 1 };
        entityRegistry.insert(std::make_pair(entity.id, newEntityRecord));
        return entity.id;
    }

    Entity World::GetEntity(Id entityId) {
        return Entity(entityId);
    }

}