//
// Created by gaetz on 13/06/23.
//

#include "EntityRingBuffer.hpp"
#include "GMath.hpp"
#include "ECSManager.hpp"

EntityRingBuffer::EntityRingBuffer(u64 sizeP, sptr<ECSManager> ecsP) :
    size { sizeP },
    ecs { std::move(ecsP) }
{
    /// TODO Assert size is superior to 1

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
    Vector2 spawnPos = ecs->GetComponent<Transform2D>(spawnerId).pos;
    // Create components if not transform, reuse else
    if (entity.components[0] == -1) {
        auto& projectileTransform = ecs->CreateTransform2DComponent(projectileId);
        const Sprite& projectileSprite = ecs->CreateSpriteComponent(projectileId, "projectile");
        auto& projectileBody = ecs->CreateRigidbody2DComponent(projectileId, Vector2 { 0, 0 },
                                                               Rectangle { 0, 0, projectileSprite.srcRect.width, projectileSprite.srcRect.height },
                                                               true, false);
        projectileTransform.pos = spawnPos + Vector2 { 32.f, 32.f };
        projectileBody.velocity = Vector2 { static_cast<float>(gmath::CosRad(radianShootAngle) * 5000.0f),
                                            static_cast<float>(gmath::SinRad(radianShootAngle) * 5000.0f) };
        LOG(LogLevel::Trace) << "Created entity " << projectileId;
    } else {
        auto& projectileTransform = ecs->GetComponent<Transform2D>(projectileId);
        auto& projectileBody = ecs->GetComponent<Rigidbody2D>(projectileId);
        projectileTransform.pos = spawnPos + Vector2 { 32.f, 32.f };
        projectileBody.velocity = Vector2 { static_cast<float>(gmath::CosRad(radianShootAngle) * 5000.0f),
                                            static_cast<float>(gmath::SinRad(radianShootAngle) * 5000.0f) };
        LOG(LogLevel::Trace) << "Reused entity " << projectileId;
    }
    return projectileId;
}


