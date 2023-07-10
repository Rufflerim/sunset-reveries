//
// Created by gaetz on 03/07/2023.
//

#include "World.hpp"

namespace gecs {

    void World::MoveEntity(Id entity, Archetype* archetype, size_t row, Archetype* nextArchetype) {
        /// TODO
        // Insert in new archetype data from previous archetype
        for (IColumn& dstCol : nextArchetype->components) {
            for (IColumn& srcCol : archetype->components) {
                if (dstCol.id == srcCol.id) {
                    void* data = srcCol.GetDataRow(row);
                    dstCol.AddElement(data, srcCol.GetDataSize());
                    // Update entity's row ?

                }
            }
        }
    }
}