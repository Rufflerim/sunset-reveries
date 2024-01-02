//
// Created by gaetz on 03/07/2023.
//

#include "World.hpp"
#include "Entity.hpp"
#include "Log.hpp"
#include "StringUtils.hpp"
#include <sstream>

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
        archetypeRegistry[positionArchetypeId].archetypeChanges[ComponentId::Position].remove = &archetypeRegistry[emptyArchetypeId];;
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

    u64 World::MoveEntity(const ArchetypeRecord& recordToUpdate, size_t row, Archetype* nextArchetype) {
        u64 checkRow = std::numeric_limits<uint64_t>::max();
        u64 newRow = std::numeric_limits<uint64_t>::max();
        i32 checkColsDst { 0 }; // Used to avoid empty archetype case
        i32 checkColsSrc { 0 }; // Used to avoid empty archetype case

        // Insert in new archetype data from previous archetype
        for (Column& dstCol: nextArchetype->components) {
            for (Column& srcCol: recordToUpdate.archetype->components) {
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
                if (checkRow != std::numeric_limits<uint64_t>::max()) {
                    GASSERT_MSG(checkRow == newRow, "Row must be the same in all archetype columns");
                }
                checkRow = newRow;

                // Remove previous data from archetype, after saving data
                srcCol.RemoveElement(row);
                ++checkColsSrc;
            }
            ++checkColsDst;
        }
        GASSERT_MSG(newRow != std::numeric_limits<uint64_t>::max() && checkColsDst > 0 && checkColsSrc > 0,
                    "Row should exist");
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


    vector<vector<std::pair<ArchetypeId, size_t>>> World::GetArchetypeAndColumnIndices(vector<CompArchIdAndCol> &compArchCols) {
        vector<vector<std::pair<ArchetypeId, size_t>>> ret;
        ComponentId currentCompId = compArchCols[0].componentId;;
        vector<std::pair<ArchetypeId, size_t>> currentArchsAndCols { {compArchCols[0].archId, compArchCols[0].columnIndex} };
        for (u32 i = 1; i < compArchCols.size(); ++i) {
            if (currentCompId != compArchCols[i].componentId) {
                ret.emplace_back(currentArchsAndCols);
                currentArchsAndCols.clear();
                currentArchsAndCols.emplace_back(compArchCols[i].archId, compArchCols[i].columnIndex);
                currentCompId = compArchCols[i].componentId;
                continue;
            }
            currentArchsAndCols.emplace_back(compArchCols[i].archId, compArchCols[i].columnIndex);
        }
        ret.emplace_back(currentArchsAndCols);
        return ret;
    }

    vector<vector<size_t>> World::GetDataStartIndices(vector<CompArchIdAndCol> &compArchCols) {
        ComponentId currentCompId = compArchCols[0].componentId;;
        vector<vector<size_t>> starts {};
        vector<size_t> currentStarts { 0 };
        size_t accumulator { 0 };
        // We create a vector of start indices for each component.
        // We count the number of elements for the current archetype
        // and add it to the accumulator, so that we can know the start index
        // for a specific archetype for the same component.
        for (u32 i = 1; i < compArchCols.size(); ++i) {
            if (currentCompId != compArchCols[i].componentId) {
                starts.emplace_back(currentStarts);
                currentStarts.clear();
                currentStarts.push_back(0);
                accumulator = 0;
                currentCompId = compArchCols[i].componentId;
                continue;
            }
            accumulator += archetypeRegistry[compArchCols[i].archId].components[compArchCols[i].columnIndex].Count();
            currentStarts.push_back(accumulator);
        }
        starts.emplace_back(currentStarts);
        return starts;
    }
}