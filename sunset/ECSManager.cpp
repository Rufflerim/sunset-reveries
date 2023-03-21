//
// Created by gaetz on 11/11/2022.
//

#include "ECSManager.h"
#include "Renderer.h"
#include "WorldChange.h"
#include "GMath.h"

u32 ECSManager::maxId { 0 };

void ECSManager::UpdateScene(f32 dt) {
    SystemPhysicsUpdate(dt);
    //SystemScreenBounceUpdate();
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
                                            const Rectangle& box, bool doApplyGravity) {
    i32 newComponentId = static_cast<i32>(bodies.size());
    bodies.emplace_back(entityId, pos, box, doApplyGravity);
    UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::Rigidbody2D);
}

void ECSManager::CreateBodyRaycast2DComponent(u32 entityId, std::shared_ptr<ECSManager> ecs,
                                              i32 horizontalRaysCount, i32 verticalRaysCount,
                                              f32 horizontalRayLength, f32 verticalRayLength,
                                              f32 margin) {
    i32 newComponentId = static_cast<i32>(bodyRays.size());
    bodyRays.emplace_back(entityId, ecs, horizontalRaysCount, verticalRaysCount,
                          horizontalRayLength, verticalRayLength, margin);
    UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::BodyRaycast2D);
}


void ECSManager::UpdateEntityWithComponent(u32 entityId, i32 newComponentId, ComponentIndex componentIndex) {
    i32 iComponentIndex = static_cast<i32>(componentIndex);
    FindEntity(entityId).components[iComponentIndex] = newComponentId;
}

void ECSManager::SystemSpriteDraw() {
    for (auto& sprite : sprites) {
        render::DrawSprite(sprite.tex, sprite.srcRect, sprite.dstRect, WHITE);
    }
#ifdef GDEBUG
    for (auto& raycast : bodyRays) {
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
}

void ECSManager::SystemPhysicsUpdate(float dt) {
    // Raycasting
    for (const auto& raycast : bodyRays) {
        for (const auto& body : bodies) {
            if (body.entityId == raycast.entityId) continue;
            for (const auto& ray : raycast.horizontalRays) {
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
    collisionChanges.reserve(bodies.size());
    for (const auto& body : bodies) {
        if (!body.doApplyGravity) continue;
        // Apply velocity
        float deltaX = body.velocity.x * dt;
        float deltaY = body.velocity.y * dt;
        // Friction and gravity
        f32 gravityEffect = body.isGrounded ? 0.0f : 2000.0f * dt;
        PositionChange positionChange { body.entityId, false,
                                        { deltaX, deltaY },
                                        { body.velocity.x * -0.05f, gravityEffect }
        };
        /*
        positionChange.isGrounded = body.pos.y + positionChange.positionDelta.y >= 600.f - body.boundingBox.height;
        f32 velocityDeltaY = positionChange.isGrounded ? 0.0f : 2000.0f * dt;
        positionChange.velocityDelta.y = velocityDeltaY;

        */
        positionChanges.emplace_back(positionChange);

        /*
        // Collision test
        const Rectangle currentBox { body.pos.x + deltaX + body.boundingBox.x,
                               body.pos.y + deltaY + body.boundingBox.y,
                               body.boundingBox.width, body.boundingBox.height
        };
        for (const auto& other: bodies) {
            if (body.entityId == other.entityId) continue;
            const Rectangle otherBox { other.pos.x + other.boundingBox.x,
                                 other.pos.y + other.boundingBox.y,
                                 other.boundingBox.width, other.boundingBox.height
            };
            if (CheckCollisionRecs(currentBox, otherBox)) {
                //collisions.emplace_back(body.entityId, currentBox, body.velocity, body.entityId, otherBox, body.velocity);
                CollisionChange collisionChange {
                        body.entityId,
                        Vector2 { -body.velocity.x * dt, -body.velocity.y * dt },
                        Vector2 { -body.velocity.x, -body.velocity.y }
                };
                collisionChanges.emplace_back(collisionChange);
            }
        }
         */
    }

    // Change position changes in function of collisions
    for (const auto& raycastCollision : raycastCollisions) {
        if (raycastCollision.lengthSquaredBeforeCollision <= 25.0f) { // This is margin TODO improve harcoded value
            const f32 directionX = raycastCollision.ray.direction.x;
            const f32 directionY = raycastCollision.ray.direction.y;
            auto itr = std::find_if(positionChanges.begin(), positionChanges.end(),
                                    [&](const PositionChange& positionChange) { return positionChange.entityId == raycastCollision.entityId; });

            if (directionX > 0) {
                itr->velocityDelta.x = 0;
                itr->positionDelta.x = raycastCollision.otherBody.GetRealX() - (raycastCollision.emitterBody.GetRealX() + raycastCollision.emitterBody.boundingBox.width);
            } else if (directionX < 0) {
                itr->velocityDelta.x = 0;
                itr->positionDelta.x = (raycastCollision.emitterBody.GetRealX() + raycastCollision.emitterBody.boundingBox.width) - raycastCollision.otherBody.GetRealX();
            }
            if (directionY > 0) {
                //itr->velocityDelta.y = 0;
                itr->isGrounded = true;
                if (!raycastCollision.emitterBody.isGrounded) {
                    itr->positionDelta.y = raycastCollision.otherBody.GetRealY() -
                                           (raycastCollision.emitterBody.GetRealY() +
                                            raycastCollision.emitterBody.boundingBox.height);
                }
            }
        }
    }
    raycastCollisions.clear();
}

WorldState ECSManager::UpdateWorld() {
    // Player
    bool hasJumped = false;
    for (auto playerChange: playerChanges) {
        auto& body = GetComponent<Rigidbody2D>(playerChange.entityId);
        body.velocity = body.velocity + playerChange.velocityDelta;
        if (playerChange.velocityDelta.y < 0) {
            hasJumped = true;
        }
    }

    // Position
    for (auto positionChange: positionChanges) {
        auto &transform = GetComponent<Transform2D>(positionChange.entityId);
        auto &body = GetComponent<Rigidbody2D>(positionChange.entityId);

        if (positionChange.isGrounded) {
            if (!hasJumped) {
                body.isGrounded = true;
            }
        }

        if (positionChange.isGrounded && positionChange.velocityDelta.y > 0) {
            body.velocity.y = 0.0f;
            positionChange.velocityDelta.y = 0.0f;
        }

        transform.pos = transform.pos + positionChange.positionDelta;
        body.pos = transform.pos;
        body.velocity = body.velocity + positionChange.velocityDelta;

        if (body.velocity.y < 0) {
            body.isGrounded = false;
        }
    }
    // Raycast update
    for (auto& raycast : bodyRays) {
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
    // Collisions
    /*
    for (auto collisionChange: collisionChanges) {
        auto& transform = GetComponent<Transform2D>(collisionChange.entityId);
        auto& body = GetComponent<Rigidbody2D>(collisionChange.entityId);
        transform.pos = transform.pos + collisionChange.positionDelta;
        body.pos = transform.pos;
        body.velocity = collisionChange.newVelocity;
    }
    // Bounces
    for (auto bounceChange: bounceChanges) {
        auto& transform = GetComponent<Transform2D>(bounceChange.entityId);
        auto& body = GetComponent<Rigidbody2D>(bounceChange.entityId);
        transform.pos.y = bounceChange.newY;
        body.pos.y = transform.pos.y;
        body.velocity.y = bounceChange.newVerticalVelocity;
    }
     */

    positionChanges.clear();
    collisionChanges.clear();
    bounceChanges.clear();

    WorldState newWorldState {
        entityIds, entities, transforms, sprites, bodies
    };
    return newWorldState;

}