//
// Created by gaetz on 13/06/23.
//

/*

#include "EntityRingBuffer.hpp"
#include "Vec2.hpp"
#include "ECSManager.hpp"
#include "Asserts.hpp"

using gmath::Vec2;

EntityRingBuffer::EntityRingBuffer(u64 sizeP, sptr<ECSManager> ecsP) :
    size { sizeP },
    ecs { std::move(ecsP) }
{
    GASSERT_MSG(sizeP > 1, "Entity ring buffer size should be superior to 1")

    entityFirstId = ecs->CreateEntity();
    for (u64 i = 0; i < size - 1; ++i) {
        ecs->CreateEntity();
    }
    pointer = entityFirstId;
}

u64 EntityRingBuffer::NextEntity() {
    u64 value = pointer;
    pointer = ((pointer - entityFirstId + 1) % size) + entityFirstId;
    return value;
}

void EntityRingBuffer::FreeEntity(u64 entityId) {
    ecs->RemoveEntity(entityId);
}

u64 EntityRingBuffer::CreateProjectile(u64 spawnerId, f32 radianShootAngle) {
    u64 projectileId = NextEntity();
    Entity& entity = ecs->FindEntity(projectileId);
    Vec2 spawnPos = ecs->GetComponent<Transform2D>(spawnerId).pos;
    // Create components if not transform, reuse else
    if (entity.components[0] == -1) {
        auto& projectileTransform = ecs->CreateTransform2DComponent(projectileId);
        const Sprite& projectileSprite = ecs->CreateSpriteComponent(projectileId, "projectile");
        auto& projectileBody = ecs->CreateRigidbody2DComponent(projectileId, Vec2 { 0, 0 },
                                                               Rect { 0, 0, projectileSprite.srcRect.width, projectileSprite.srcRect.height },
                                                               true, false);
        projectileTransform.pos = spawnPos + Vec2 { 32, 32 };
        projectileBody.velocity = Vec2 { gmath::CosRad(radianShootAngle) * 5000.0f,
                                            gmath::SinRad(radianShootAngle) * 5000.0f };
        LOG(LogLevel::Trace) << "Created entity " << projectileId;
    } else {
        auto& projectileTransform = ecs->GetComponent<Transform2D>(projectileId);
        auto& projectileBody = ecs->GetComponent<Rigidbody2D>(projectileId);
        projectileTransform.pos = spawnPos + Vec2 { 32, 32 };
        projectileBody.velocity = Vec2 { gmath::CosRad(radianShootAngle) * 5000.0f,
                                            gmath::SinRad(radianShootAngle) * 5000.0f };
        LOG(LogLevel::Trace) << "Reused entity " << projectileId;
    }
    return projectileId;
}

 */

