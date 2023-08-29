//
// Created by gaetz on 11/11/2022.
//

#ifndef SUNSET_REVERIES_ECS_MANAGER_HPP
#define SUNSET_REVERIES_ECS_MANAGER_HPP

#include "Defines.hpp"
#include "Entity.hpp"
#include "Components.hpp"
#include "WorldChanger.hpp"
#include "WorldState.hpp"

#include <variant>
#include <algorithm>

class Vec2;

class ECSManager {
public:
    ECSManager();

    void Update(f32 dt, WorldChanger& worldChanger);
    void RenderWorld();

    [[nodiscard]] WorldState GetCurrentWorldState() const;
    void SetWorldState(const WorldState& newWorldState);
    void SetCurrentFrame(u32 currentFrameP) {
        currentFrame = currentFrameP;
    }

    void EndUpdate();
    void PrepareDraw();

    u64 CreateEntity();
    void RemoveEntity(u64 entityId);
    Entity& FindEntity(u64 entityId);

    Transform2D& CreateTransform2DComponent(u64 entityId);
    Sprite& CreateSpriteComponent(u64 entityId, const str& texName);
    Rigidbody2D& CreateRigidbody2DComponent(u64 entityId, const Vec2& pos, const Rect& box,
                                    bool doApplyGravity, bool isGhost);
    RigidbodyRaycast2D& CreateBodyRaycast2DComponent(u64 entityId, const sptr<ECSManager>& ecs,
                                      i32 horizontalRaysCount, i32 verticalRaysCount,
                                      f32 horizontalRayLength, f32 verticalRayLength, f32 margin
                                      );
    Replay& CreateReplayComponent(u64 entityId, u64 formerEntityId, u32 startFrame, u32 endFrame);
    Weapon& CreateWeaponComponent(u64 entityId, EntityRingBuffer* projectileRingBuffer);

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
        } else if constexpr (std::is_same_v<T, Weapon>) {
            return weapons.at(FindEntityComponent(entityId, ComponentIndex::Weapon));
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
    vector<Weapon> weapons;

    u32 currentFrame { 0 };
    vector<RaycastCollision> raycastCollisions;

    i32 FindEntityComponent(u64 entityId, ComponentIndex componentIndex);
    void UpdateEntityWithComponent(u64 entityId, i32 newComponentId, ComponentIndex componentIndex);
    void CleanRemovedEntities();

    void SystemPhysicsUpdate(f32 dt, WorldChanger& worldChanger);
    void SystemReplayUpdate();
    void SystemWeaponUpdate(f32 dt);
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
        } else if constexpr (std::is_same_v<T, Weapon>) {
            RemoveComponent<Weapon>(weapons, removedEntity, ComponentIndex::Weapon);
        }
    }
};

#endif //SUNSET_REVERIES_ECS_MANAGER_HPP
