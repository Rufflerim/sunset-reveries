//
// Created by gaetz on 11/11/2022.
//

#ifndef SUNSET_REVERIES_ECSMANAGER_H
#define SUNSET_REVERIES_ECSMANAGER_H

#include "Defines.h"
#include "Entity.h"
#include "Components.h"
#include <vector>

using std::vector;

/// TODO Same update function is called from any scene class
/// - Workaround: create one public update function per calling class, good for very simple game
/// - Implement a strategy pattern and inject a friend class with and update and draw function

class ECSManager {
public:
    void Update(f32 dt);
    void Draw();

    u32 CreateEntity();
    void RemoveEntity(u32 entityId);
    void CleanRemovedEntities();
    Entity& FindEntity(u32 entityId);

    void CreateTransform2DComponent(u32 entityId);
    void CreateSpriteComponent(u32 entityId, const str& texName);
    void CreateRigidbody2DComponent(u32 entityId, const Rectangle& box);

    Transform2D& GetTransform2DComponent(u32 entityId);
    Sprite& GetSpriteComponent(u32 entityId);
    Rigidbody2D& GetRigidbody2DComponent(u32 entityId);

private:
    vector<Entity> entities;
    vector<Transform2D> transforms;
    vector<Sprite> sprites;
    vector<Rigidbody2D> bodies;

    vector<u32> entitiesToRemove {};

    void UpdateEntityWithComponent(u32 entityId, i32 newComponentId, ComponentIndices componentIndex);
    void UpdateComponentsWithTransform();

    void SystemPhysicsUpdate(float dt);
    void SystemScreenBounceUpdate();
    void SystemSpriteDraw();
};


#endif //SUNSET_REVERIES_ECSMANAGER_H
