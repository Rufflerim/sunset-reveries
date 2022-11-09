//
// Created by gaetz on 06/11/2022.
//

#include "SceneGame.h"
#include "../gaemi/Renderer.h"
#include "../gaemi/AssetsManager.h"


void SceneGame::Load() {
    AssetsManager::LoadTexture("bg_sunset", "assets/bg_sunset.png", ToSceneId(SceneName::SceneGame));
    texture = AssetsManager::GetTexture("bg_sunset");
    AssetsManager::LoadTexture("player", "assets/player.png", ToSceneId(SceneName::SceneGame));

    u32 playerId = CreateEntity();
    CreateTransform2DComponent(playerId);
    GetTransform2DComponent(playerId).pos.x = 400;
    GetTransform2DComponent(playerId).pos.y = 400;

    CreateSpriteComponent(playerId, "player");

    const auto& playerTexture = AssetsManager::GetTexture("player");
    CreateRigidbody2DComponent(playerId, { 0, 0, static_cast<float>(playerTexture.width), static_cast<float>(playerTexture.height)});
    GetRigidbody2DComponent(playerId).velocity.x = 500.0f;
    GetRigidbody2DComponent(playerId).velocity.y = 500.0f;

    for (i32 i = 0; i < 15; ++i) {
        CreateRandomBouncingEntity();
    }
}

void SceneGame::Update(float dt) {
    SystemPhysicsUpdate(dt);
    SystemScreenBounceUpdate();
}

void SceneGame::Draw() {
    render::DrawTexture(texture, 0, 120, WHITE);
    SystemSpriteDraw();
}

void SceneGame::Unload() {
    AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}

u32 SceneGame::CreateEntity() {
    u32 newId = entities.size();
    entities.emplace_back(newId);
    return newId;
}

u32 SceneGame::CreateRandomBouncingEntity() {
    u32 newId = entities.size();
    entities.emplace_back(newId);

    CreateTransform2DComponent(newId);
    GetTransform2DComponent(newId).pos.x = static_cast<float>(GetRandomValue(100, 1100));
    GetTransform2DComponent(newId).pos.y = static_cast<float>(GetRandomValue(150, 550));

    CreateSpriteComponent(newId, "player");

    const auto& playerTexture = AssetsManager::GetTexture("player");
    CreateRigidbody2DComponent(newId, { 0, 0, static_cast<float>(playerTexture.width), static_cast<float>(playerTexture.height)});
    GetRigidbody2DComponent(newId).velocity.x = static_cast<float>(GetRandomValue(0, 600));
    GetRigidbody2DComponent(newId).velocity.y = static_cast<float>(GetRandomValue(0, 600));

    return newId;
}

void SceneGame::CreateTransform2DComponent(u32 entityId) {
    i32 newComponentId = static_cast<i32>(transforms.size());
    transforms.emplace_back(Transform2D(entityId));
    UpdateEntityWithComponent(entityId, newComponentId, ComponentIndices::Transform2D);
}

void SceneGame::CreateSpriteComponent(u32 entityId, const str& texName) {
    i32 newComponentId = static_cast<i32>(sprites.size());
    const Texture& tex = AssetsManager::GetTexture(texName);
    sprites.emplace_back(Sprite(entityId, texName, static_cast<float>(tex.width), static_cast<float>(tex.height)));
    UpdateEntityWithComponent(entityId, newComponentId, ComponentIndices::Sprite);
}

void SceneGame::CreateRigidbody2DComponent(u32 entityId, const Rectangle& box) {
    i32 newComponentId = static_cast<i32>(bodies.size());
    bodies.emplace_back(Rigidbody2D(entityId, box));
    UpdateEntityWithComponent(entityId, newComponentId, ComponentIndices::Rigidbody2D);
}

void SceneGame::UpdateEntityWithComponent(u32 entityId, i32 newComponentId, ComponentIndices componentIndex) {
    i32 iComponentIndex = static_cast<i32>(componentIndex);
    entities[entityId].components[iComponentIndex] = newComponentId;
}



void SceneGame::SystemSpriteDraw() {
    for (auto& sprite : sprites) {
        const Texture& tex = AssetsManager::GetTexture(sprite.texName.data());
        auto& transform = GetTransform2DComponent(sprite.entityId);
        sprite.dstRect = { transform.pos.x, transform.pos.y, static_cast<float>(tex.width), static_cast<float>(tex.height) };
        render::DrawSprite(tex, sprite.srcRect, sprite.dstRect, WHITE);
    }
}

void SceneGame::SystemPhysicsUpdate(float dt) {
    vector<Collision2D> collisions;
    for (auto itr = bodies.begin(); itr != bodies.end(); ++itr) {
        // Apply velocity
        auto& transform = GetTransform2DComponent(itr->entityId);
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
            auto& otherTransform = GetTransform2DComponent(other->entityId);
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
        auto& body = GetRigidbody2DComponent(collision.entityId);
        auto& tranform = GetTransform2DComponent(collision.entityId);
        body.velocity.x = -collision.velocity.x;
        body.velocity.y = -collision.velocity.y;
        tranform.pos.x += body.velocity.x * dt * 2;
        tranform.pos.y += body.velocity.y * dt * 2;
    }
}

void SceneGame::SystemScreenBounceUpdate() {
    for (auto& body : bodies) {
        auto& transform = GetTransform2DComponent(body.entityId);
        Rectangle currentBox { transform.pos.x + body.boundingBox.x,
                               transform.pos.y + body.boundingBox.y,
                               body.boundingBox.width, body.boundingBox.height
                            };
        float blackRibbonHeight { 120.0f };
        float screenWidth { 1280.0f };
        float screenHeight { 720.0f };
        // Horizontal bounce
        if (currentBox.x + currentBox.width > screenWidth) {
            transform.pos.x = screenWidth - currentBox.width;
            body.velocity.x *= -1;
        } else if (currentBox.x < 0.0f) {
            transform.pos.x = 0.0f;
            body.velocity.x *= -1;
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



Transform2D& SceneGame::GetTransform2DComponent(u32 entityId) {
    return transforms.at(entities.at(entityId).components.at(static_cast<i32>(ComponentIndices::Transform2D)));
}

Sprite& SceneGame::GetSpriteComponent(u32 entityId) {
    return sprites.at(entities.at(entityId).components.at(static_cast<i32>(ComponentIndices::Sprite)));
}

Rigidbody2D& SceneGame::GetRigidbody2DComponent(u32 entityId) {
    return bodies.at(entities.at(entityId).components.at(static_cast<i32>(ComponentIndices::Rigidbody2D)));
}


