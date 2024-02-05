//
// Created by gaetz on 03/07/2023.
//

#include "World.hpp"
#include "Entity.hpp"
#include "Log.hpp"
#include "StringUtils.hpp"
#include <sstream>
#include <limits>

namespace gecs {

    void World::Init() {
        // Init archetype ids with bits
        ArchetypeId emptyArchetypeId;

        ArchetypeId positionArchetypeId;
        positionArchetypeId.flip(0);

        ArchetypeId velocityArchetypeId;
        velocityArchetypeId.flip(1);

        ArchetypeId spriteArchetypeId;
        spriteArchetypeId.flip(2);

        ArchetypeId posVelArchetypeId;
        posVelArchetypeId.flip(0).flip(1);

        ArchetypeId posSprArchetypeId;
        posSprArchetypeId.flip(0).flip(2);

        ArchetypeId velSprArchetypeId;
        velSprArchetypeId.flip(1).flip(2);

        ArchetypeId posVelSprArchetypeId;
        posVelSprArchetypeId.flip(0).flip(1).flip(2);

        // Init archetypes
        Archetype emptyArchetype;
        emptyArchetype.id = 0;
        emptyArchetype.archetypeId = emptyArchetypeId;
        gplatform::StrCpy(emptyArchetype.name, 6, "Empty");
        archetypeRegistry.insert(std::make_pair(emptyArchetypeId, std::move(emptyArchetype)));
        defaultArchetype = emptyArchetypeId;

        // Position archetype
        Archetype positionArchetype;
        positionArchetype.id = 1;
        positionArchetype.archetypeId = positionArchetypeId;
        gplatform::StrCpy(positionArchetype.name, 4, "Pos");
        Column posArchCol;
        posArchCol.Init<Position>(100);
        positionArchetype.components.push_back(std::move(posArchCol));
        archetypeRegistry.insert(std::make_pair(positionArchetypeId, std::move(positionArchetype)));

        componentRegistry[ComponentId::Position].insert(std::make_pair(positionArchetypeId, 0));

        // Velocity archetype
        Archetype velocityArchetype;
        velocityArchetype.id = 2;
        velocityArchetype.archetypeId = velocityArchetypeId;
        gplatform::StrCpy(velocityArchetype.name, 4, "Vel");
        Column velArchCol;
        velArchCol.Init<Velocity>(100);
        velocityArchetype.components.push_back(std::move(velArchCol));
        archetypeRegistry.insert(std::make_pair(velocityArchetypeId, std::move(velocityArchetype)));

        componentRegistry[ComponentId::Velocity].insert(std::make_pair(velocityArchetypeId, 0));

        // Sprite archetype
        Archetype spriteArchetype;
        spriteArchetype.id = 3;
        spriteArchetype.archetypeId = spriteArchetypeId;
        gplatform::StrCpy(spriteArchetype.name, 11, "Background");
        Column sprArchCol;
        sprArchCol.Init<Sprite>(100);
        spriteArchetype.components.push_back(std::move(sprArchCol));
        archetypeRegistry.insert(std::make_pair(spriteArchetypeId, std::move(spriteArchetype)));

        componentRegistry[ComponentId::Sprite].insert(std::make_pair(spriteArchetypeId, 0));


        // Pos Vel Archetype
        Archetype posVelArchetype;
        posVelArchetype.id = 4;
        posVelArchetype.archetypeId = posVelArchetypeId;
        gplatform::StrCpy(posVelArchetype.name, 7, "PosVel");
        Column posVelArchPosCol;
        posVelArchPosCol.Init<Position>(100);
        posVelArchetype.components.push_back(std::move(posVelArchPosCol));
        Column posVelArchVelCol;
        posVelArchVelCol.Init<Velocity>(100);
        posVelArchetype.components.push_back(std::move(posVelArchVelCol));
        archetypeRegistry.insert(std::make_pair(posVelArchetypeId, std::move(posVelArchetype)));

        componentRegistry[ComponentId::Position].insert(std::make_pair(posVelArchetypeId, 0));
        componentRegistry[ComponentId::Velocity].insert(std::make_pair(posVelArchetypeId, 1));

        // Pos Sprite Archetype
        Archetype posSprArchetype;
        posSprArchetype.id = 5;
        posSprArchetype.archetypeId = posSprArchetypeId;
        gplatform::StrCpy(posSprArchetype.name, 10, "FixSprite");
        Column posSprArchPosCol;
        posSprArchPosCol.Init<Position>(100);
        posSprArchetype.components.push_back(std::move(posSprArchPosCol));
        Column posSprArchSprCol;
        posSprArchSprCol.Init<Sprite>(100);
        posSprArchetype.components.push_back(std::move(posSprArchSprCol));
        archetypeRegistry.insert(std::make_pair(posSprArchetypeId, std::move(posSprArchetype)));

        componentRegistry[ComponentId::Position].insert(std::make_pair(posSprArchetypeId, 0));
        componentRegistry[ComponentId::Sprite].insert(std::make_pair(posSprArchetypeId, 1));

        // Vel Sprite Archetype
        Archetype velSprArchetype;
        velSprArchetype.id = 6;
        velSprArchetype.archetypeId = velSprArchetypeId;
        gplatform::StrCpy(velSprArchetype.name, 7, "VelSpr");
        Column velSprArchVelCol;
        velSprArchVelCol.Init<Position>(100);
        velSprArchetype.components.push_back(std::move(velSprArchVelCol));
        Column velSprArchSprCol;
        velSprArchSprCol.Init<Sprite>(100);
        velSprArchetype.components.push_back(std::move(velSprArchSprCol));
        archetypeRegistry.insert(std::make_pair(velSprArchetypeId, std::move(velSprArchetype)));

        componentRegistry[ComponentId::Velocity].insert(std::make_pair(velSprArchetypeId, 0));
        componentRegistry[ComponentId::Sprite].insert(std::make_pair(velSprArchetypeId, 1));

        // Pos Vel Sprite Archetype
        Archetype posVelSprArchetype;
        posVelSprArchetype.id = 7;
        posVelSprArchetype.archetypeId = posVelSprArchetypeId;
        gplatform::StrCpy(posVelSprArchetype.name, 7, "Sprite");
        Column posVelSprArchPosCol;
        posVelSprArchPosCol.Init<Position>(100);
        posVelSprArchetype.components.push_back(std::move(posVelSprArchPosCol));
        Column posVelSprArchVelCol;
        posVelSprArchVelCol.Init<Velocity>(100);
        posVelSprArchetype.components.push_back(std::move(posVelSprArchVelCol));
        Column posVelSprArchSprCol;
        posVelSprArchSprCol.Init<Sprite>(100);
        posVelSprArchetype.components.push_back(std::move(posVelSprArchSprCol));
        archetypeRegistry.insert(std::make_pair(posVelSprArchetypeId, std::move(posVelSprArchetype)));

        componentRegistry[ComponentId::Position].insert(std::make_pair(posVelSprArchetypeId, 0));
        componentRegistry[ComponentId::Velocity].insert(std::make_pair(posVelSprArchetypeId, 1));
        componentRegistry[ComponentId::Sprite].insert(std::make_pair(posVelSprArchetypeId, 2));


        // Init archetype graph
        // -- Empty
        archetypeRegistry[emptyArchetypeId].archetypeChanges[ComponentId::Position].add = &archetypeRegistry[positionArchetypeId];
        archetypeRegistry[emptyArchetypeId].archetypeChanges[ComponentId::Position].remove = nullptr;
        archetypeRegistry[emptyArchetypeId].archetypeChanges[ComponentId::Velocity].add = &archetypeRegistry[velocityArchetypeId];
        archetypeRegistry[emptyArchetypeId].archetypeChanges[ComponentId::Velocity].remove = nullptr;
        archetypeRegistry[emptyArchetypeId].archetypeChanges[ComponentId::Sprite].add = &archetypeRegistry[spriteArchetypeId];
        archetypeRegistry[emptyArchetypeId].archetypeChanges[ComponentId::Sprite].remove = nullptr;
        // -- Position
        archetypeRegistry[positionArchetypeId].archetypeChanges[ComponentId::Position].add = &archetypeRegistry[positionArchetypeId];
        archetypeRegistry[positionArchetypeId].archetypeChanges[ComponentId::Position].remove = &archetypeRegistry[emptyArchetypeId];
        archetypeRegistry[positionArchetypeId].archetypeChanges[ComponentId::Velocity].add = &archetypeRegistry[posVelArchetypeId];
        archetypeRegistry[positionArchetypeId].archetypeChanges[ComponentId::Velocity].remove = nullptr;
        archetypeRegistry[positionArchetypeId].archetypeChanges[ComponentId::Sprite].add = &archetypeRegistry[posSprArchetypeId];
        archetypeRegistry[positionArchetypeId].archetypeChanges[ComponentId::Sprite].remove = nullptr;
        // -- Velocity
        archetypeRegistry[velocityArchetypeId].archetypeChanges[ComponentId::Position].add = &archetypeRegistry[posVelArchetypeId];
        archetypeRegistry[velocityArchetypeId].archetypeChanges[ComponentId::Position].remove = nullptr;
        archetypeRegistry[velocityArchetypeId].archetypeChanges[ComponentId::Velocity].add = &archetypeRegistry[velocityArchetypeId];
        archetypeRegistry[velocityArchetypeId].archetypeChanges[ComponentId::Velocity].remove = &archetypeRegistry[emptyArchetypeId];
        archetypeRegistry[velocityArchetypeId].archetypeChanges[ComponentId::Sprite].add = &archetypeRegistry[velSprArchetypeId];
        archetypeRegistry[velocityArchetypeId].archetypeChanges[ComponentId::Sprite].remove = nullptr;
        // -- Sprite
        archetypeRegistry[spriteArchetypeId].archetypeChanges[ComponentId::Position].add = &archetypeRegistry[posSprArchetypeId];
        archetypeRegistry[spriteArchetypeId].archetypeChanges[ComponentId::Position].remove = nullptr;
        archetypeRegistry[spriteArchetypeId].archetypeChanges[ComponentId::Velocity].add = &archetypeRegistry[velSprArchetypeId];
        archetypeRegistry[spriteArchetypeId].archetypeChanges[ComponentId::Velocity].remove = nullptr;
        archetypeRegistry[spriteArchetypeId].archetypeChanges[ComponentId::Sprite].add = &archetypeRegistry[spriteArchetypeId];
        archetypeRegistry[spriteArchetypeId].archetypeChanges[ComponentId::Sprite].remove = &archetypeRegistry[emptyArchetypeId];
        // -- Position and Velocity
        archetypeRegistry[posVelArchetypeId].archetypeChanges[ComponentId::Position].add = &archetypeRegistry[posVelArchetypeId];
        archetypeRegistry[posVelArchetypeId].archetypeChanges[ComponentId::Position].remove = &archetypeRegistry[velocityArchetypeId];
        archetypeRegistry[posVelArchetypeId].archetypeChanges[ComponentId::Velocity].add = &archetypeRegistry[posVelArchetypeId];
        archetypeRegistry[posVelArchetypeId].archetypeChanges[ComponentId::Velocity].remove = &archetypeRegistry[positionArchetypeId];
        archetypeRegistry[posVelArchetypeId].archetypeChanges[ComponentId::Sprite].add = &archetypeRegistry[posVelSprArchetypeId];
        archetypeRegistry[posVelArchetypeId].archetypeChanges[ComponentId::Sprite].remove = &archetypeRegistry[posVelArchetypeId];
        // -- Position and Sprite
        archetypeRegistry[posSprArchetypeId].archetypeChanges[ComponentId::Position].add = &archetypeRegistry[posSprArchetypeId];
        archetypeRegistry[posSprArchetypeId].archetypeChanges[ComponentId::Position].remove = &archetypeRegistry[spriteArchetypeId];
        archetypeRegistry[posSprArchetypeId].archetypeChanges[ComponentId::Velocity].add = &archetypeRegistry[posVelSprArchetypeId];
        archetypeRegistry[posSprArchetypeId].archetypeChanges[ComponentId::Velocity].remove = &archetypeRegistry[posSprArchetypeId];
        archetypeRegistry[posSprArchetypeId].archetypeChanges[ComponentId::Sprite].add = &archetypeRegistry[posSprArchetypeId];
        archetypeRegistry[posSprArchetypeId].archetypeChanges[ComponentId::Sprite].remove = &archetypeRegistry[positionArchetypeId];
        // -- Velocity and Sprite
        archetypeRegistry[velSprArchetypeId].archetypeChanges[ComponentId::Position].add = &archetypeRegistry[posVelSprArchetypeId];
        archetypeRegistry[velSprArchetypeId].archetypeChanges[ComponentId::Position].remove = &archetypeRegistry[velSprArchetypeId];
        archetypeRegistry[velSprArchetypeId].archetypeChanges[ComponentId::Velocity].add = &archetypeRegistry[velSprArchetypeId];
        archetypeRegistry[velSprArchetypeId].archetypeChanges[ComponentId::Velocity].remove = &archetypeRegistry[spriteArchetypeId];
        archetypeRegistry[velSprArchetypeId].archetypeChanges[ComponentId::Sprite].add = &archetypeRegistry[velSprArchetypeId];
        archetypeRegistry[velSprArchetypeId].archetypeChanges[ComponentId::Sprite].remove = &archetypeRegistry[velocityArchetypeId];
        // -- Position, Velocity and Sprite
        archetypeRegistry[posVelSprArchetypeId].archetypeChanges[ComponentId::Position].add = &archetypeRegistry[posVelSprArchetypeId];
        archetypeRegistry[posVelSprArchetypeId].archetypeChanges[ComponentId::Position].remove = &archetypeRegistry[velSprArchetypeId];
        archetypeRegistry[posVelSprArchetypeId].archetypeChanges[ComponentId::Velocity].add = &archetypeRegistry[posVelSprArchetypeId];
        archetypeRegistry[posVelSprArchetypeId].archetypeChanges[ComponentId::Velocity].remove = &archetypeRegistry[posSprArchetypeId];
        archetypeRegistry[posVelSprArchetypeId].archetypeChanges[ComponentId::Sprite].add = &archetypeRegistry[posVelSprArchetypeId];
        archetypeRegistry[posVelSprArchetypeId].archetypeChanges[ComponentId::Sprite].remove = &archetypeRegistry[posVelArchetypeId];
    }

    Id World::CreateEntity() {
        auto entity = Entity(maxId++);
        ArchetypeRecord newEntityRecord { &archetypeRegistry[defaultArchetype], 0 /* No column */ };
        entityRegistry.insert(std::make_pair(entity.id, newEntityRecord));
        return entity.id;
    }

    Entity World::GetEntity(Id entityId) {
        return Entity(entityId);
    }

    void World::RemoveComponent(Id entityId, ComponentId componentId) {
        ArchetypeRecord& recordToUpdate = GetEntities()[entityId];
        Archetype* nextArchetype = recordToUpdate.archetype->archetypeChanges[componentId].remove;
        // Move previous archetype data in new archetype
        u64 newRow = MoveEntity(recordToUpdate, nextArchetype, true);
        // Update entity's row
        recordToUpdate.archetype = nextArchetype;
        recordToUpdate.row = newRow;
    }

    u64 World::MoveEntity(const ArchetypeRecord& recordToUpdate, Archetype* nextArchetype, bool remove) {
        size_t row = recordToUpdate.row;
        u64 checkRow = std::numeric_limits<u64>::max();
        u64 newRow = std::numeric_limits<u64>::max();
        i32 checkColsDst { 0 }; // Used to avoid empty archetype case
        i32 checkColsSrc { 0 }; // Used to avoid empty archetype case

        auto currentArchetype = recordToUpdate.archetype;
        const u64 lastRow = currentArchetype->GetRowCount() - 1;
        const auto entityRecordFromLastRow = std::find_if(entityRegistry.begin(), entityRegistry.end(),
                                                          [=](const std::pair<Id, ArchetypeRecord>& record) {
                                                              return record.second.archetype == currentArchetype &&
                                                                     record.second.row == lastRow;
                                                          });
        vector<ComponentId> unmovedComponents = ToComponentIds(currentArchetype->archetypeId);

        // Insert in new archetype data from previous archetype
        for (Column& dstCol: nextArchetype->components) {
            for (Column& srcCol: currentArchetype->components) {
                if (dstCol.GetComponentId() != srcCol.GetComponentId()) continue;

                // Copy data in new component columns
                switch (dstCol.GetComponentId()) {
                    case ComponentId::Position: {
                        const auto component = srcCol.GetRowConst<Position>(row);
                        newRow = dstCol.AddElement<Position>(component);
                    }
                        break;
                    case ComponentId::Velocity: {
                        const auto component = srcCol.GetRowConst<Velocity>(row);
                        newRow = dstCol.AddElement<Velocity>(component);
                    }
                        break;
                    case ComponentId::Sprite: {
                        const auto component = srcCol.GetRowConst<Sprite>(row);
                        newRow = dstCol.AddElement<Sprite>(component);
                    }
                        break;
                }

                // Check row is the same for each column
                if (checkRow != std::numeric_limits<u64>::max()) {
                    GASSERT_MSG(checkRow == newRow, "Row must be the same in all archetype columns")
                }
                checkRow = newRow;

                // Remove previous data from archetype, after saving data
                srcCol.RemoveElementBySwapping(row);
                unmovedComponents.erase(std::remove(unmovedComponents.begin(), unmovedComponents.end(), dstCol.GetComponentId()), unmovedComponents.end());
                entityRecordFromLastRow->second.row = row;
                ++checkColsSrc;
            }
            ++checkColsDst;
        }
        GASSERT_MSG(newRow != std::numeric_limits<u64>::max() && checkColsDst > 0 && checkColsSrc > 0,
                    "Row should exist")

        // Remove remaining component from previous archetype
        if (remove) {
            GASSERT_MSG(unmovedComponents.size() == 1, "Only one component should remain");
            const ComponentId componentId = unmovedComponents[0];
            currentArchetype->components[static_cast<i32>(componentId)].RemoveElementBySwapping(row);
        }

        return newRow;
    }

    void World::LogWorld() {
        std::stringstream stream;
        stream << std::endl;
        stream << Log::LogMap<gecs::Id, gecs::ArchetypeRecord>("Entities", entityRegistry,
                                                   [&](gecs::Id id, const gecs::ArchetypeRecord& record) {
                                                       str strId = "Entity: "+ std::to_string(id);
                                                       str archetypeId = "Archetype: " + ArchetypeIdToString(record.archetype->archetypeId);
                                                       str strRow = "Row: " + std::to_string(record.row);
                                                       str strComp;
                                                       Archetype* entityArchetype = record.archetype;
                                                       for (Column& column : entityArchetype->components) {
                                                           ComponentId componentId = column.GetComponentId();
                                                           strComp += ComponentIdToString(componentId);
                                                           strComp += ": ";
                                                           strComp += LogComponent(componentId, column, record.row);
                                                       }
                                                       return strId + " | " + archetypeId + " | " + strRow + " | " + strComp;
                                                   });

        stream << std::endl;
        stream << Log::LogMap<gecs::ArchetypeId, gecs::Archetype>("Archetypes", archetypeRegistry,
                                                   [](gecs::ArchetypeId id, const gecs::Archetype& archetype) {
                                                       str archetypeId = "Archetype: " + ArchetypeIdToString(id);
                                                       str count = "Number of entities: " + std::to_string(archetype.GetRowCount());
                                                       return archetypeId + " | " + count;
                                                   });
        LOG(LogLevel::Debug) << "WORLD STATUS" << std::endl << stream.str();
    }

    Archetype* World::GetArchetype(const str& archetypeName) {
        for (auto& a : archetypeRegistry) {
            if (a.second.name == archetypeName) {
                return &a.second;
            }
        }
        return nullptr;
    }

    void World::DestroyEntity(Id entityId) {
        auto& entityRecord = entityRegistry[entityId];
        Archetype* archetype = entityRecord.archetype;
        const u64 row = entityRecord.row;
        const u64 lastRow = archetype->GetRowCount() - 1;
        const auto entityRecordFromLastRow = std::find_if(entityRegistry.begin(), entityRegistry.end(),
                                                          [=](const std::pair<Id, ArchetypeRecord>& record) {
                                                              return record.second.archetype == archetype &&
                                                                     record.second.row == lastRow;
                                                          });
        // Remove column data by swapping with last row
        // then update the entity record
        for (auto& column : archetype->components) {
            column.RemoveElementBySwapping(row);
        }
        entityRecordFromLastRow->second.row = row;
        // Erase entity from registry
        entityRegistry.erase(entityId);
    }
}