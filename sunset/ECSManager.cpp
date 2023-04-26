//
// Created by gaetz on 11/11/2022.
//

#include <algorithm>
#include <limits>

#include "ECSManager.hpp"
#include "Renderer.hpp"
#include "WorldChange.hpp"
#include "GMath.hpp"
#include "Jobs.hpp"

u64 ECSManager::maxId { 0 };

ECSManager::ECSManager() :
    PLAYER_GHOST_FADE_TIME { static_cast<i32>(AssetsManager::GetData("PLAYER_GHOST_FADE_TIME")) },
    PHYSICS_RAYCAST_MARGIN { AssetsManager::GetData("PHYSICS_RAYCAST_MARGIN") },
    PHYSICS_GRAVITY_ACCELERATION { AssetsManager::GetData("PHYSICS_GRAVITY_ACCELERATION") },
    PHYSICS_FRICTION_RATE { AssetsManager::GetData("PHYSICS_FRICTION_RATE") }
{

}


void ECSManager::UpdateScene(f32 dt) {
    SystemReplayUpdate();
    jobs::Execute([this, dt] { SystemWeaponUpdate(dt); });
    jobs::Execute([this, dt] { SystemPhysicsUpdate(dt); });
    jobs::Wait();
}

void ECSManager::DrawScene() {
    SystemSpriteDraw();

    CleanRemovedEntities();
}

u64 ECSManager::CreateEntity() {
    u64 newId = maxId++;
    entityIds.emplace_back(newId);
    entities.emplace_back(newId);
    return newId;
}

Transform2D& ECSManager::CreateTransform2DComponent(u64 entityId) {
    i32 newComponentId = static_cast<i32>(transforms.size());
    transforms.emplace_back(entityId);
    UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::Transform2D);
    return transforms.back();
}

Sprite& ECSManager::CreateSpriteComponent(u64 entityId, const str& texName) {
    i32 newComponentId = static_cast<i32>(sprites.size());
    const Texture& tex = AssetsManager::GetTexture(texName);
    sprites.emplace_back(entityId, texName, static_cast<float>(tex.width), static_cast<float>(tex.height));
    UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::Sprite);
    return sprites.back();
}

Rigidbody2D& ECSManager::CreateRigidbody2DComponent(u64 entityId, const Vector2& pos,
                                            const Rectangle& box, bool doApplyGravity, bool isGhost) {
    i32 newComponentId = static_cast<i32>(bodies.size());
    bodies.emplace_back(entityId, pos, box, doApplyGravity, isGhost);
    UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::Rigidbody2D);
    return bodies.back();
}

RigidbodyRaycast2D& ECSManager::CreateBodyRaycast2DComponent(u64 entityId, const std::shared_ptr<ECSManager>& ecs,
                                              i32 horizontalRaysCount, i32 verticalRaysCount,
                                              f32 horizontalRayLength, f32 verticalRayLength,
                                              f32 margin) {
    i32 newComponentId = static_cast<i32>(bodyRaycasts.size());
    bodyRaycasts.emplace_back(entityId, ecs, horizontalRaysCount, verticalRaysCount,
                              horizontalRayLength, verticalRayLength, margin);
    UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::BodyRaycast2D);
    return bodyRaycasts.back();
}

Replay& ECSManager::CreateReplayComponent(u64 entityId, u64 formerEntityId, u32 startFrame, u32 endFrame) {
    i32 newComponentId = static_cast<i32>(replays.size());
    replays.emplace_back(entityId, formerEntityId, startFrame, endFrame);
    UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::Replay);
    return replays.back();
}

Weapon& ECSManager::CreateWeaponComponent(u64 entityId, shared_ptr<ECSManager> ecs) {
    i32 newComponentId = static_cast<i32>(weapons.size());
    weapons.emplace_back(entityId, std::move(ecs));
    UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::Weapon);
    return weapons.back();
}



void ECSManager::UpdateEntityWithComponent(u64 entityId, i32 newComponentId, ComponentIndex componentIndex) {
    i32 iComponentIndex = static_cast<i32>(componentIndex);
    FindEntity(entityId).components[iComponentIndex] = newComponentId;
}

void ECSManager::SystemSpriteDraw() {
    for (auto& sprite : sprites) {
        Color colorAlpha { 255, 255, 255, sprite.opacity };
        render::DrawSprite(sprite.tex, sprite.srcRect, sprite.dstRect, colorAlpha);
    }
#ifdef GDEBUG
    for (auto& raycast : bodyRaycasts) {
        raycast.DrawDebug();
    }
    for (auto& body : bodies) {
        body.DrawDebug();
    }
#endif
}

void ECSManager::RemoveEntity(u64 entityId) {
    entitiesToRemove.push_back(entityId);
}

void ECSManager::CleanRemovedEntities() {
    for (auto entityId : entitiesToRemove) {
        // Transform
        RemoveEntityComponent<Transform2D>(entityId);
        // Sprites
        RemoveEntityComponent<Sprite>(entityId);
        // Rigidbodies
        RemoveEntityComponent<Rigidbody2D>(entityId);
        // Raycasts
        RemoveEntityComponent<RigidbodyRaycast2D>(entityId);
        // Replay
        RemoveEntityComponent<Replay>(entityId);

        std::erase(entityIds, entityId);
        std::erase_if(entities, [=](Entity entity) {
            return entity.id == entityId;
        });
    }
    entitiesToRemove.clear();
}

Entity& ECSManager::FindEntity(u64 entityId) {
    auto itr = std::lower_bound(entityIds.begin(), entityIds.end(), entityId);
    return entities.at(std::distance(entityIds.begin(), itr));
}

i32 ECSManager::FindEntityComponent(u64 entityId, ComponentIndex componentIndex) {
    return FindEntity(entityId).components.at(static_cast<i32>(componentIndex));
}


void ECSManager::PrepareDraw() {
    // Update sprite positions
    for (auto& sprite : sprites) {
        const auto& transform = GetComponent<Transform2D>(sprite.entityId);
        sprite.dstRect = { transform.pos.x, transform.pos.y, static_cast<float>(sprite.tex.width), static_cast<float>(sprite.tex.height) };
    }
}

void ECSManager::SetWorldState(const WorldState &newWorldState) {
    entityIds = newWorldState.entityIds;
    entities = newWorldState.entities;
    transforms = newWorldState.transforms;
    sprites = newWorldState.sprites;
    bodies = newWorldState.bodies;
    replays = newWorldState.replays;
}

void ECSManager::SystemPhysicsUpdate(float dt) {
    // Ray casting
    for (const auto& raycast : bodyRaycasts) {
        for (const auto& body : bodies) {
            if (body.entityId == raycast.entityId) continue;
            for (const auto& ray : raycast.horizontalRays) {
                // No horizontal collisions with ghosts
                if (body.isGhost) continue;
                const Rectangle bodyRect = body.GetPositionedRectangle();
                Vector2 contactPoint;
                Vector2 contactNormal;
                float contactTime;
                std::array<Vector2, 2> probableContactPoints {};
                if (gmath::RayVsRect2D(ray.origin, ray.direction, bodyRect, contactPoint, contactNormal, contactTime, probableContactPoints)) {
                    const Vector2 diff = contactPoint - ray.origin;
                    raycastCollisions.emplace_back(raycast.entityId, body.entityId,
                                                   contactPoint,raycast.attachBody, body,
                                                   ray, diff.x * diff.x + diff.y * diff.y);
                }
            }
            for (const auto& ray : raycast.verticalRays) {
                const Rectangle bodyRect = body.GetPositionedRectangle();
                Vector2 contactPoint;
                Vector2 contactNormal;
                float contactTime;
                std::array<Vector2, 2> probableContactPoints {};
                if (gmath::RayVsRect2D(ray.origin, ray.direction, bodyRect, contactPoint, contactNormal, contactTime, probableContactPoints)) {
                    const Vector2 diff = contactPoint - ray.origin;
                    raycastCollisions.emplace_back(raycast.entityId, body.entityId,
                                                   contactPoint,raycast.attachBody, body,
                                                   ray, diff.x * diff.x + diff.y * diff.y);
                }
            }
        }
    }

    // Newtonian physics
    positionChanges.reserve(transforms.size());
    for (const auto& body : bodies) {
        if (!body.doApplyGravity) continue;
        if (body.isGhost) continue;
        // Friction and gravity
        f32 gravityEffect = PHYSICS_GRAVITY_ACCELERATION * dt;
        PositionChange positionChange { body.entityId, false,
                                        { -body.velocity.x * PHYSICS_FRICTION_RATE, gravityEffect },
                                        dt
        };
        positionChanges.emplace_back(positionChange);
    }

    // Change position changes in function of collisions
    f32 minDistance = std::numeric_limits<f32>::max();
    u32 raycastCheckedEntity = std::numeric_limits<u32>::max();
    // We consider raycastCollisions are sorted by raycasting entity id
    for (const auto& raycastCollision : raycastCollisions) {
        if (raycastCollision.entityId != raycastCheckedEntity) {
            raycastCheckedEntity = raycastCollision.entityId;
            minDistance = std::numeric_limits<f32>::max();
        }
        const Rigidbody2D& emitterBody = raycastCollision.emitterBody;
        const Rigidbody2D& otherBody = raycastCollision.otherBody;
        const f32 raycastMarginSquared = PHYSICS_RAYCAST_MARGIN * PHYSICS_RAYCAST_MARGIN;
        const f32 checkValue = std::max(raycastMarginSquared, std::max(emitterBody.velocity.x, emitterBody.velocity.y));
        if (raycastCollision.lengthSquaredBeforeCollision <= checkValue && raycastCollision.lengthSquaredBeforeCollision < minDistance) {
            minDistance = raycastCollision.lengthSquaredBeforeCollision;
            const f32 directionX = raycastCollision.ray.direction.x;
            const f32 directionY = raycastCollision.ray.direction.y;
            auto itr = std::find_if(positionChanges.begin(), positionChanges.end(),
                                    [&](const PositionChange& positionChange) { return positionChange.entityId == raycastCollision.entityId; });

            if (directionX > 0 && emitterBody.velocity.x > 0) {
                itr->stopVelocityX = true;
                itr->positionXFixAfterCollision = otherBody.GetRealX() - emitterBody.boundingBox.width;
            } else if (directionX < 0 && emitterBody.velocity.x < 0) {
                itr->stopVelocityX = true;
                itr->positionXFixAfterCollision = otherBody.GetRealX() + otherBody.boundingBox.width;
            }
            if (directionY > 0 && emitterBody.velocity.y >= 0) {
                itr->stopVelocityY = true;
                itr->isGrounded = true;
                itr->positionYFixAfterCollision = otherBody.GetRealY() - emitterBody.boundingBox.height;
                itr->groundVelocity = otherBody.velocity;
            } else if (directionY < 0 && emitterBody.velocity.y < 0 && !otherBody.isGhost) {
                itr->stopVelocityY = true;
                itr->positionYFixAfterCollision = otherBody.GetRealY() + otherBody.boundingBox.height;
            }
        }
    }
    raycastCollisions.clear();
}

void ECSManager::SystemReplayUpdate() {
    for (const auto &replay: replays) {
        if (currentFrame >= replay.replayEndFrame) {
            /// TODO If by rewind / forward we go back to a time when a ghost is disappearing, it may disappear immediatly because replayEndFrame has changed
            // Ghost fades after lifetime
            GetComponent<Sprite>(replay.entityId).opacity = static_cast<u8>(static_cast<f32>(replay.replayEndFrame + PLAYER_GHOST_FADE_TIME - currentFrame) / static_cast<f32>(PLAYER_GHOST_FADE_TIME) * 255.0f);
            GetComponent<Rigidbody2D>(replay.entityId).velocity = { 0, 0 };
            if (currentFrame >= replay.replayEndFrame + PLAYER_GHOST_FADE_TIME) {
                RemoveEntity(replay.entityId);
            }
            continue;
        }
        GetComponent<Transform2D>(replay.entityId) = replay.transforms.at(currentFrame - replay.replayStartFrame);
        GetComponent<Rigidbody2D>(replay.entityId) = replay.bodies.at(currentFrame - replay.replayStartFrame);
    }
}

void ECSManager::SystemWeaponUpdate(f32 dt) {
    for (auto& weapon : weapons) {
        weapon.Update(dt);
    }
}


WorldState ECSManager::UpdateWorld() {
    // Player
    for (auto playerChange: playerChanges) {
        auto& body = GetComponent<Rigidbody2D>(playerChange.entityId);
        body.playerAcceleration = playerChange.velocityDelta;
    }

    // Position
    for (const auto positionChange: positionChanges) {
        auto &transform = GetComponent<Transform2D>(positionChange.entityId);
        auto &body = GetComponent<Rigidbody2D>(positionChange.entityId);

        // Manage moving ground
        body.isGrounded = positionChange.isGrounded;
        body.groundVelocity = positionChange.groundVelocity;
        if (body.playerAcceleration.y < 0) {
            body.isGrounded = false;
            // Compensate ground move for more control
            body.playerAcceleration = body.playerAcceleration - body.groundVelocity;
        }

        // Manage velocity and player movement
        body.velocity = body.velocity + positionChange.velocityDelta + body.playerAcceleration;
        body.playerAcceleration = {0, 0 };

        if (positionChange.stopVelocityX) {
            body.velocity.x = 0;
        }
        if (positionChange.stopVelocityY && body.isGrounded && (body.velocity.y > 0 || body.velocity.y < 0)) {
            body.velocity.y = 0;
        }

        transform.pos = transform.pos + body.velocity * positionChange.dt + positionChange.groundVelocity * positionChange.dt;
        if (positionChange.positionXFixAfterCollision != -1.0f) {
            transform.pos.x = positionChange.positionXFixAfterCollision;
        }
        if (positionChange.positionYFixAfterCollision != -1.0f && body.isGrounded) {
            transform.pos.y = positionChange.positionYFixAfterCollision;
        }
        body.pos = transform.pos;
    }

    // Raycast update
    for (auto& raycast : bodyRaycasts) {
        if (raycast.attachBody.velocity.x > 0) {
            raycast.SetRayDirection(Ray2DDirection::Right);
        } else if (raycast.attachBody.velocity.x < 0) {
            raycast.SetRayDirection(Ray2DDirection::Left);
        }
        if (raycast.attachBody.velocity.y > 0) {
            raycast.SetRayDirection(Ray2DDirection::Down);
        } else if (raycast.attachBody.velocity.y < 0) {
            raycast.SetRayDirection(Ray2DDirection::Up);
        }
        raycast.Update();
    }

    positionChanges.clear();

    /// TODO Problem: I store all replay frames each frame
    /// Maybe replays should be stored in the world manager and data fetched from there
    WorldState newWorldState {
        0,
        entityIds, entities, transforms, sprites, bodies, replays
    };
    return newWorldState;

}


