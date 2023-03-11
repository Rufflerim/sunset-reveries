//
// Created by gaetz on 11/11/2022.
//

#ifndef SUNSET_REVERIES_ECSMANAGER_H
#define SUNSET_REVERIES_ECSMANAGER_H

#include "Defines.h"
#include "Entity.h"
#include "Components.h"
#include "WorldChange.h"
#include "WorldState.h"
#include <vector>
#include <variant>
#include <memory>

using std::vector;
using std::shared_ptr;

/// TODO Same update function is called from any scene class
/// - Workaround: create one public update function per calling class, good for very simple game
/// - Implement a strategy pattern and inject a friend class with and update and draw function

class ECSManager {
public:
    void UpdateScene(f32 dt);
    void DrawScene();

    WorldState UpdateWorld();
    void SetWorldState(const WorldState& newWorldState);
    void PrepareDraw();

    u32 CreateEntity();
    void RemoveEntity(u32 entityId);
    Entity& FindEntity(u32 entityId);

    void CreateTransform2DComponent(u32 entityId);
    void CreateSpriteComponent(u32 entityId, const str& texName);
    void CreateRigidbody2DComponent(u32 entityId, const Vector2& pos, const Rectangle& box);

    template<class T>
    T& GetComponent(u32 entityId) {
        if constexpr (std::is_same_v<T, Transform2D>) {
            return transforms.at(FindEntityComponent(entityId, ComponentIndex::Transform2D));
        } else if constexpr (std::is_same_v<T, Sprite>) {
            return sprites.at(FindEntityComponent(entityId, ComponentIndex::Sprite));
        } else if constexpr (std::is_same_v<T, Rigidbody2D>) {
            return bodies.at(FindEntityComponent(entityId, ComponentIndex::Rigidbody2D));
        }
    }

private:
    static u32 maxId;

    vector<u32> entityIds;
    vector<Entity> entities;
    vector<Transform2D> transforms;
    vector<Sprite> sprites;
    vector<Rigidbody2D> bodies;

    vector<u32> entitiesToRemove {};

    vector<CollisionChange> collisionChanges;
    vector<PositionChange> positionChanges;
    vector<BounceChange> bounceChanges;

    i32 FindEntityComponent(u32 entityId, ComponentIndex componentIndex);
    void UpdateEntityWithComponent(u32 entityId, i32 newComponentId, ComponentIndex componentIndex);
    void CleanRemovedEntities();

    void SystemPhysicsUpdate(float dt);
    void SystemScreenBounceUpdate();

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
    void RemoveEntityComponent(u32 entityId) {
        auto& removedEntity = FindEntity(entityId);
        if constexpr (std::is_same_v<T, Transform2D>) {
            RemoveComponent<Transform2D>(transforms, removedEntity, ComponentIndex::Transform2D);
        } else if constexpr (std::is_same_v<T, Sprite>) {
            RemoveComponent<Sprite>(sprites, removedEntity, ComponentIndex::Sprite);
        } else if constexpr (std::is_same_v<T, Rigidbody2D>) {
            RemoveComponent<Rigidbody2D>(bodies, removedEntity, ComponentIndex::Rigidbody2D);
        }
    }
};


#endif //SUNSET_REVERIES_ECSMANAGER_H
