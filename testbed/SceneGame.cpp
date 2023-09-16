//
// Created by gaetz on 06/11/2022.
//

#include "SceneGame.hpp"
#include <utility>
#include <iomanip>
#include "Renderer.hpp"
#include "../gecs/World.hpp"
#include "../gecs/Entity.hpp"
#include "AssetsManager.hpp"

using gecs::Position;
using gecs::Velocity;

SceneGame::SceneGame(Game& game)
: game { game }
{

}

void SceneGame::Load() {
    AssetsManager::LoadTexture("bg_sunset", "assets/images/bg_sunset.png", ToSceneId(SceneName::SceneGame));
    AssetsManager::LoadTexture("player", "assets/images/player.png", ToSceneId(SceneName::SceneGame));
    AssetsManager::LoadTexture("ghost", "assets/images/ghost.png", ToSceneId(SceneName::SceneGame));
    AssetsManager::LoadTexture("projectile", "assets/images/projectile.png", ToSceneId(SceneName::SceneGame));

    backgroundTexture = AssetsManager::GetTexture("bg_sunset");

    gecs::World& world = gecs::World::Instance();
    world.Init();
    testEntityId = world.CreateEntity();
    gecs::Entity entity = world.GetEntity(testEntityId);


    Position pos {2, 5};
    entity.AddComponent<gecs::Position>(pos);
    Velocity vel {1, 0};
    entity.AddComponent<gecs::Velocity>(vel);
    gecs::Sprite sprite { AssetsManager::GetTexture("player") };
    entity.AddComponent<gecs::Sprite>(sprite);

    world.LogWorld();


}

void SceneGame::Update(f32 dt) {
    gecs::World& world = gecs::World::Instance();
    Position position = world.GetEntity(testEntityId).GetComponent<Position>();


}


void SceneGame::Draw() {
    render::DrawTexture(backgroundTexture, 0, 120, WHITE);
}


void SceneGame::Unload() {
    AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}

u64 SceneGame::CreateRandomBouncingEntity() {

    /*
    u64 newId = ecs->CreateEntity();

    const f32 x = static_cast<f32>(GetRandomValue(100, 1100));
    const f32 y = static_cast<f32>(GetRandomValue(150, 550));

    auto& transform = ecs->CreateTransform2DComponent(newId);
    transform.pos = Vec2 { x, y };

    ecs->CreateSpriteComponent(newId, "player");
    const auto& playerTexture = AssetsManager::GetTexture("player");

    auto& body = ecs->CreateRigidbody2DComponent(newId, Vec2 { x, y },
                                    { 0, 0, static_cast<float>(playerTexture.width), static_cast<float>(playerTexture.height)},
                                    true, false);
    body.velocity.x = static_cast<float>(GetRandomValue(0, 600));
    body.velocity.y = static_cast<float>(GetRandomValue(0, 600));

    return newId;
     */
    return 0;
}





