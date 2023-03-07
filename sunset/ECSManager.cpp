//
// Created by gaetz on 11/11/2022.
//

#include "ECSManager.h"
#include "Renderer.h"
#include "WorldChange.h"
#include "GMath.h"

void ECSManager::UpdateSceneGame(f32 dt) {
    SystemPhysicsUpdate(dt);
    SystemScreenBounceUpdate();
}

void ECSManager::DrawSceneGame() {
    SystemSpriteDraw();

    CleanRemovedEntities();
}

u32 ECSManager::CreateEntity() {
    u32 newId = entities.size();
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

void ECSManager::CreateRigidbody2DComponent(u32 entityId, const Rectangle& box) {
    i32 newComponentId = static_cast<i32>(bodies.size());
    bodies.emplace_back(entityId, box);
    UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::Rigidbody2D);
}

void ECSManager::UpdateEntityWithComponent(u32 entityId, i32 newComponentId, ComponentIndex componentIndex) {
    i32 iComponentIndex = static_cast<i32>(componentIndex);
    FindEntity(entityId).components[iComponentIndex] = newComponentId;
}

void ECSManager::UpdateComponentsWithTransform() {
    // Update sprite positions
    for (auto& sprite : sprites) {
        auto& transform = GetComponent<Transform2D>(sprite.entityId);
        sprite.dstRect = { transform.pos.x, transform.pos.y, static_cast<float>(sprite.tex.width), static_cast<float>(sprite.tex.height) };
    }
}

void ECSManager::SystemPhysicsUpdate(float dt) {
    vector<Collision2D> collisions;
    for (auto itr = bodies.begin(); itr != bodies.end(); ++itr) {
        // Apply velocity
        auto transform = GetComponent<Transform2D>(itr->entityId);
        float deltaX = itr->velocity.x * dt;
        float deltaY = itr->velocity.y * dt;
        PositionChange positionChange { itr->entityId, { deltaX, deltaY } };
        positionChanges.emplace_back(positionChange);

        // Collision test
        Rectangle currentBox { transform.pos.x + deltaX + itr->boundingBox.x,
                               transform.pos.y + deltaY + itr->boundingBox.y,
                               itr->boundingBox.width, itr->boundingBox.height
        };
        for (auto other = bodies.begin(); other != bodies.end(); ++other) {
            if (itr == other) continue;
            auto& otherTransform = GetComponent<Transform2D>(other->entityId);
            Rectangle otherBox { otherTransform.pos.x + other->boundingBox.x,
                                 otherTransform.pos.y + other->boundingBox.y,
                                 other->boundingBox.width, other->boundingBox.height
            };
            if (CheckCollisionRecs(currentBox, otherBox)) {
                //collisions.emplace_back(itr->entityId, currentBox, itr->velocity, other->entityId, otherBox, other->velocity);
                CollisionChange collisionChange {
                        itr->entityId,
                        Vector2 { -itr->velocity.x * dt, -itr->velocity.y * dt },
                        Vector2 { -itr->velocity.x, -itr->velocity.y }
                };
                collisionChanges.emplace_back(collisionChange);
            }
        }
    }
}

void ECSManager::SystemScreenBounceUpdate() {
    for (auto body : bodies) {
        auto transform = GetComponent<Transform2D>(body.entityId);
        Rectangle currentBox { transform.pos.x + body.boundingBox.x,
                               transform.pos.y + body.boundingBox.y,
                               body.boundingBox.width, body.boundingBox.height
        };
        float blackRibbonHeight { 120.0f };
        float screenWidth { 1280.0f };
        float screenHeight { 720.0f };
        // Horizontal exit
        if (currentBox.x > screenWidth) {
            RemoveEntity(body.entityId);
        } else if (currentBox.x + currentBox.width < 0.0f) {
            RemoveEntity(body.entityId);
        }
        // Vertical bounce
        if (currentBox.y + currentBox.height > screenHeight - blackRibbonHeight) {
            BounceChange bounceChange {
                body.entityId,
                screenHeight - blackRibbonHeight - currentBox.height,
                -body.velocity.y
            };
            bounceChanges.emplace_back(bounceChange);
        } else if (currentBox.y < blackRibbonHeight) {
            BounceChange bounceChange {
                    body.entityId,
                    blackRibbonHeight,
                    -body.velocity.y
            };
            bounceChanges.emplace_back(bounceChange);
        }
    }
}

void ECSManager::SystemSpriteDraw() {
    for (auto& sprite : sprites) {
        render::DrawSprite(sprite.tex, sprite.srcRect, sprite.dstRect, WHITE);
    }
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

        std::erase_if(entities, [=](Entity entity) {
            return entity.id == entityId;
        });
    }
    entitiesToRemove.clear();
}

Entity& ECSManager::FindEntity(u32 entityId) {
    auto res = std::find_if(entities.begin(), entities.end(), [entityId](Entity& entity) { return entity.id == entityId; });
    return *res;
}

i32 ECSManager::FindEntityComponent(u32 entityId, ComponentIndex componentIndex) {
    return FindEntity(entityId).components.at(static_cast<i32>(componentIndex));
}

WorldState ECSManager::UpdateWorld() {
    // Position
    for (auto positionChange : positionChanges) {
        auto& transform = GetComponent<Transform2D>(positionChange.entityId);
        transform.pos = transform.pos + positionChange.positionDelta;
    }
    // Collisions
    for (auto collisionChange : collisionChanges) {
        auto& transform = GetComponent<Transform2D>(collisionChange.entityId);
        auto& body = GetComponent<Rigidbody2D>(collisionChange.entityId);
        transform.pos = transform.pos + collisionChange.positionDelta;
        body.velocity = collisionChange.newVelocity;
    }
    for (auto bounceChange : bounceChanges) {
        auto& transform = GetComponent<Transform2D>(bounceChange.entityId);
        auto& body = GetComponent<Rigidbody2D>(bounceChange.entityId);
        transform.pos.y = bounceChange.newY;
        body.velocity.y = bounceChange.newVerticalVelocity;
    }

    positionChanges.clear();
    collisionChanges.clear();
    bounceChanges.clear();

    // Update components' data
    UpdateComponentsWithTransform();

    WorldState newWorldState {
        entities, transforms, sprites, bodies
    };
    return newWorldState;
}

void ECSManager::SetWorldState(const WorldState &newWorldState) {
    entities = newWorldState.entities;
    transforms = newWorldState.transforms;
    sprites = newWorldState.sprites;
    bodies = newWorldState.bodies;
}


