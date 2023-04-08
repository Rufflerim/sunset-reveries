//
// Created by gaetz on 11/11/2022.
//

#ifndef SUNSET_REVERIES_ECS_MANAGER_HPP
#define SUNSET_REVERIES_ECS_MANAGER_HPP

#include "Defines.hpp"
#include "Entity.hpp"
#include "Components.hpp"
#include "WorldChange.hpp"
#include "WorldState.hpp"
#include <vector>
#include <variant>
#include <memory>

using std::vector;
using std::shared_ptr;

class ECSManager {
public:
    ECSManager();

    void UpdateScene(f32 dt);
    void DrawScene();

    void SetPlayerChanges(const vector<PlayerChange>& playerChangesP) { playerChanges = playerChangesP; }
    WorldState UpdateWorld();
    void SetWorldState(const WorldState& newWorldState);
    void PrepareDraw();
    void SetCurrentFrame(u32 currentFrameP) {
        currentFrame = currentFrameP;
    }

    u64 CreateEntity();
    void RemoveEntity(u64 entityId);
    Entity& FindEntity(u64 entityId);

    void CreateTransform2DComponent(u64 entityId);
    void CreateSpriteComponent(u64 entityId, const str& texName);
    void CreateRigidbody2DComponent(u64 entityId, const Vector2& pos, const Rectangle& box,
                                    bool doApplyGravity, bool isGhost);
    void CreateBodyRaycast2DComponent(u64 entityId, const std::shared_ptr<ECSManager>& ecs,
                                      i32 horizontalRaysCount, i32 verticalRaysCount,
                                      f32 horizontalRayLength, f32 verticalRayLength, f32 margin
                                      );
    void CreateReplayComponent(u64 entityId, u64 formerEntityId, u32 startFrame, u32 endFrame);

    template<class T>
    T& GetComponent(u64 entityId) {
        if constexpr (std::is_same_v<T, Transform2D>) {
            return transforms.at(FindEntityComponent(entityId, ComponentIndex::Transform2D));
        } else if constexpr (std::is_same_v<T, Sprite>) {
            return sprites.at(FindEntityComponent(entityId, ComponentIndex::Sprite));
        } else if constexpr (std::is_same_v<T, Rigidbody2D>) {
            return bodies.at(FindEntityComponent(entityId, ComponentIndex::Rigidbody2D));
        } else if constexpr (std::is_same_v<T, RigidbodyRaycast2D>) {
            return bodyRaycasts.at(FindEntityComponent(entityId, ComponentIndex::BodyRaycast2D));
        } else if constexpr (std::is_same_v<T, Replay>) {
            return replays.at(FindEntityComponent(entityId, ComponentIndex::Replay));
        }
    }

private:
    static u64 maxId;
    const i32 PLAYER_GHOST_FADE_TIME;
    const f32 PHYSICS_RAYCAST_MARGIN;
    const f32 PHYSICS_GRAVITY_ACCELERATION;
    const f32 PHYSICS_FRICTION_RATE;

    // Entities and components
    vector<u64> entityIds;
    vector<Entity> entities;
    vector<u64> entitiesToRemove {};

    vector<Transform2D> transforms;
    vector<Sprite> sprites;
    vector<Rigidbody2D> bodies;
    vector<RigidbodyRaycast2D> bodyRaycasts;
    vector<Replay> replays;

    // World updates
    u32 currentFrame { 0 };
    vector<PositionChange> positionChanges;
    vector<PlayerChange> playerChanges;
    vector<RaycastCollision> raycastCollisions;

    i32 FindEntityComponent(u64 entityId, ComponentIndex componentIndex);
    void UpdateEntityWithComponent(u64 entityId, i32 newComponentId, ComponentIndex componentIndex);
    void CleanRemovedEntities();

    void SystemPhysicsUpdate(f32 dt);
    void SystemReplayUpdate();
    void SystemSpriteDraw();

    template<class T>
    void RemoveComponent(vector<T>& components, Entity& removedEntity, ComponentIndex componentTypeIndex) {
        i32 typeIndex = static_cast<i32>(componentTypeIndex);
        auto componentIndex = removedEntity.components.at(typeIndex);
        if (componentIndex != -1) {
            auto last = components.end() - 1;
            FindEntity(last->entityId).components[typeIndex] = componentIndex;
            auto removedComponent = components.begin() + componentIndex;
            std::iter_swap(removedComponent, last);
            components.pop_back();
        }
    }

    template<class T>
    void RemoveEntityComponent(u64 entityId) {
        auto& removedEntity = FindEntity(entityId);
        if constexpr (std::is_same_v<T, Transform2D>) {
            RemoveComponent<Transform2D>(transforms, removedEntity, ComponentIndex::Transform2D);
        } else if constexpr (std::is_same_v<T, Sprite>) {
            RemoveComponent<Sprite>(sprites, removedEntity, ComponentIndex::Sprite);
        } else if constexpr (std::is_same_v<T, Rigidbody2D>) {
            RemoveComponent<Rigidbody2D>(bodies, removedEntity, ComponentIndex::Rigidbody2D);
        } else if constexpr (std::is_same_v<T, RigidbodyRaycast2D>) {
            RemoveComponent<RigidbodyRaycast2D>(bodyRaycasts, removedEntity, ComponentIndex::BodyRaycast2D);
        } else if constexpr (std::is_same_v<T, Replay>) {
            RemoveComponent<Replay>(replays, removedEntity, ComponentIndex::Replay);
        }
    }
};


#endif //SUNSET_REVERIES_ECS_MANAGER_HPP
