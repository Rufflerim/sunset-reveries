//
// Created by gaetz on 13/06/23.
//

/*

#ifndef SUNSET_REVERIES_ENTITY_RING_BUFFER_HPP
#define SUNSET_REVERIES_ENTITY_RING_BUFFER_HPP

#include "Defines.hpp"

class ECSManager;

class EntityRingBuffer {
public:
    EntityRingBuffer(u64 size, sptr<ECSManager> ecs);
    u64 NextEntity();
    void FreeEntity(u64 entityId);

    u64 CreateProjectile(u64 spawnerId, f32 radianShootAngle);

private:
    u64 entityFirstId;
    u64 size;
    u64 pointer;
    sptr<ECSManager> ecs;
};


#endif //SUNSET_REVERIES_ENTITY_RING_BUFFER_HPP

 */