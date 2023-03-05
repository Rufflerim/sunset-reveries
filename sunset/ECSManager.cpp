//
// Created by gaetz on 11/11/2022.
//

#include "ECSManager.h"
#include "Renderer.h"

void ECSManager::UpdateSceneGame(f32 dt) {
    SystemPhysicsUpdate(dt);
    SystemScreenBounceUpdate();

    // Update components' data
    UpdateComponentsWithTransform();
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
    transforms.emplace_back(Transform2D(entityId));
    UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::Transform2D);
}

void ECSManager::CreateSpriteComponent(u32 entityId, const str& texName) {
    i32 newComponentId = static_cast<i32>(sprites.size());
    const Texture& tex = AssetsManager::GetTexture(texName);
    sprites.emplace_back(Sprite(entityId, texName, static_cast<float>(tex.width), static_cast<float>(tex.height)));
    UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::Sprite);
}

void ECSManager::CreateRigidbody2DComponent(u32 entityId, const Rectangle& box) {
    i32 newComponentId = static_cast<i32>(bodies.size());
    bodies.emplace_back(Rigidbody2D(entityId, box));
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
        auto& transform = GetComponent<Transform2D>(itr->entityId);
        float newX = transform.pos.x + itr->velocity.x * dt;
        float newY = transform.pos.y + itr->velocity.y * dt;
        transform.pos = { newX, newY };
        // Collision test
        Rectangle currentBox { newX + itr->boundingBox.x,
                               newY + itr->boundingBox.y,
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
                collisions.emplace_back(itr->entityId, currentBox, itr->velocity, other->entityId, otherBox, other->velocity);
            }
        }
    }
    // Manage collisions
    for (auto& collision : collisions) {
        auto& body = GetComponent<Rigidbody2D>(collision.entityId);
        auto& tranform = GetComponent<Transform2D>(collision.entityId);
        body.velocity.x = -collision.velocity.x;
        body.velocity.y = -collision.velocity.y;
        tranform.pos.x += body.velocity.x * dt * 2;
        tranform.pos.y += body.velocity.y * dt * 2;
    }
}

void ECSManager::SystemScreenBounceUpdate() {
    for (auto& body : bodies) {
        auto& transform = GetComponent<Transform2D>(body.entityId);
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
            transform.pos.y = screenHeight - blackRibbonHeight - currentBox.height;
            body.velocity.y *= -1;
        } else if (currentBox.y < blackRibbonHeight) {
            transform.pos.y = blackRibbonHeight;
            body.velocity.y *= -1;
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

