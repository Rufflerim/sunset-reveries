//
// Created by gaetz on 03/07/2023.
//

#include "World.hpp"

namespace gecs {

    void World::MoveEntity(ArchetypeRecord& recordToUpdate, size_t row, Archetype* nextArchetype) {
        /// TODO
        // Insert in new archetype data from previous archetype
        for (IColumn& dstCol : nextArchetype->components) {
            for (IColumn& srcCol : recordToUpdate.archetype->components) {
                if (dstCol.id == srcCol.id) {
                    // Copy data in new component columns
                    void* data = srcCol.GetDataRow(row);
                    auto newRow = dstCol.AddElement(data, srcCol.GetDataSize());
                    // Remove previous data from archetype, after saving data
                    srcCol.RemoveElement(row);
                    // Update entity's row
                    recordToUpdate.archetype = nextArchetype;
                    recordToUpdate.row = newRow;
                }
            }
        }
    }
}