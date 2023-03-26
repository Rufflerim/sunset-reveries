//
// Created by gaetz on 11/11/2022.
//

#include <algorithm>
#include "ECSManager.hpp"
#include "Renderer.hpp"
#include "WorldChange.hpp"
#include "GMath.hpp"

u32 ECSManager::maxId { 0 };

ECSManager::ECSManager() :
    PLAYER_GHOST_FADE_TIME { static_cast<i32>(AssetsManager::GetData("PLAYER_GHOST_FADE_TIME")) },
    PHYSICS_RAYCAST_MARGIN { AssetsManager::GetData("PHYSICS_RAYCAST_MARGIN") },
    PHYSICS_GRAVITY_ACCELERATION { AssetsManager::GetData("PHYSICS_GRAVITY_ACCELERATION") },
    PHYSICS_FRICTION_RATE { AssetsManager::GetData("PHYSICS_FRICTION_RATE") }
{

}


void ECSManager::UpdateScene(f32 dt) {
    SystemPhysicsUpdate(dt);
    SystemReplayUpdate();
}

void ECSManager::DrawScene() {
    SystemSpriteDraw();

    CleanRemovedEntities();
}

u32 ECSManager::CreateEntity() {
    u32 newId = maxId++;
    entityIds.emplace_back(newId);
    entities.emplace_back(newId);
    return newId;
}

void ECSManager::CreateTransform2DComponent(u32 entityId) {
    i32 newComponentId = static_cast<i32>(transforms.size());
    transforms.emplace_back(entityId);
    UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::Transform2D);
}

void ECSManager::CreateSpriteComponent(u32 entityId, const str& texName) {
    i32 newComponentId = static_cast<i32>(sprites.size());
    const Texture& tex = AssetsManager::GetTexture(texName);
    sprites.emplace_back(entityId, texName, static_cast<float>(tex.width), static_cast<float>(tex.height));
    UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::Sprite);
}

void ECSManager::CreateRigidbody2DComponent(u32 entityId, const Vector2& pos,
                                            const Rectangle& box, bool doApplyGravity, bool isGhost) {
    i32 newComponentId = static_cast<i32>(bodies.size());
    bodies.emplace_back(entityId, pos, box, doApplyGravity, isGhost);
    UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::Rigidbody2D);
}

void ECSManager::CreateBodyRaycast2DComponent(u32 entityId, const std::shared_ptr<ECSManager>& ecs,
                                              i32 horizontalRaysCount, i32 verticalRaysCount,
                                              f32 horizontalRayLength, f32 verticalRayLength,
                                              f32 margin) {
    i32 newComponentId = static_cast<i32>(bodyRaycasts.size());
    bodyRaycasts.emplace_back(entityId, ecs, horizontalRaysCount, verticalRaysCount,
                              horizontalRayLength, verticalRayLength, margin);
    UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::BodyRaycast2D);
}

void ECSManager::CreateReplayComponent(u32 entityId, u32 formerEntityId, u32 startFrame, u32 endFrame) {
    i32 newComponentId = static_cast<i32>(replays.size());
    replays.emplace_back(entityId, formerEntityId, startFrame, endFrame);
    UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::Replay);
}



void ECSManager::UpdateEntityWithComponent(u32 entityId, i32 newComponentId, ComponentIndex componentIndex) {
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

void ECSManager::RemoveEntity(u32 entityId) {
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

Entity& ECSManager::FindEntity(u32 entityId) {
    auto itr = std::lower_bound(entityIds.begin(), entityIds.end(), entityId);
    return entities.at(std::distance(entityIds.begin(), itr));
}

i32 ECSManager::FindEntityComponent(u32 entityId, ComponentIndex componentIndex) {
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
                if (RayVsRect2D(ray.origin, ray.direction, bodyRect, contactPoint, contactNormal, contactTime, probableContactPoints)) {
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
                if (RayVsRect2D(ray.origin, ray.direction, bodyRect, contactPoint, contactNormal, contactTime, probableContactPoints)) {
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
        // Apply velocity
        float deltaX = body.velocity.x * dt;
        float deltaY = body.velocity.y * dt;
        // Friction and gravity
        f32 gravityEffect = PHYSICS_GRAVITY_ACCELERATION * dt;
        PositionChange positionChange { body.entityId, false,
                                        { deltaX, deltaY },
                                        { -body.velocity.x * PHYSICS_FRICTION_RATE, gravityEffect }
        };
        positionChanges.emplace_back(positionChange);
    }

    // Change position changes in function of collisions
    for (const auto& raycastCollision : raycastCollisions) {
        const Rigidbody2D& emitterBody = raycastCollision.emitterBody;
        const Rigidbody2D& otherBody = raycastCollision.otherBody;
        const f32 raycastMarginSquared = PHYSICS_RAYCAST_MARGIN * PHYSICS_RAYCAST_MARGIN;
        const f32 checkValue = std::max(raycastMarginSquared, std::max(emitterBody.velocity.x, emitterBody.velocity.y));
        if (raycastCollision.lengthSquaredBeforeCollision <= checkValue) {
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
            if (directionY > 0 && emitterBody.velocity.y > 0) {
                itr->stopVelocityY = true;
                itr->isGrounded = true;
                itr->positionYFixAfterCollision = otherBody.GetRealY() - emitterBody.boundingBox.height;
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
            // Ghost fades after lifetime
            GetComponent<Sprite>(replay.entityId).opacity = static_cast<u8>(static_cast<f32>(replay.replayEndFrame + PLAYER_GHOST_FADE_TIME - currentFrame) / static_cast<f32>(PLAYER_GHOST_FADE_TIME) * 255.0f);
            if (currentFrame >= replay.replayEndFrame + PLAYER_GHOST_FADE_TIME) {
                RemoveEntity(replay.entityId);
            }
            continue;
        }
        GetComponent<Transform2D>(replay.entityId) = replay.transforms.at(currentFrame - replay.replayStartFrame);
        GetComponent<Rigidbody2D>(replay.entityId) = replay.bodies.at(currentFrame - replay.replayStartFrame);
    };
}

WorldState ECSManager::UpdateWorld() {
    // Player
    for (auto playerChange: playerChanges) {
        auto& body = GetComponent<Rigidbody2D>(playerChange.entityId);
        body.velocity = body.velocity + playerChange.velocityDelta;
    }

    // Position
    for (auto positionChange: positionChanges) {
        auto &transform = GetComponent<Transform2D>(positionChange.entityId);
        auto &body = GetComponent<Rigidbody2D>(positionChange.entityId);
        body.isGrounded = positionChange.isGrounded;

        body.velocity = body.velocity + positionChange.velocityDelta;
        if (positionChange.stopVelocityX) {
            body.velocity.x = 0;
        }
        if (positionChange.stopVelocityY && ((body.isGrounded && body.velocity.y > 0) || body.velocity.y < 0)) {
            body.velocity.y = 0;
        }

        transform.pos = transform.pos + positionChange.positionDelta;
        if (positionChange.positionXFixAfterCollision != -1.0f) {
            transform.pos.x = positionChange.positionXFixAfterCollision;
        }
        if (positionChange.positionYFixAfterCollision != -1.0f) {
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

