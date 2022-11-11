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


class ECSManager {
public:
    void Update(f32 dt);
    void Draw();

    u32 CreateEntity();

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

    void UpdateEntityWithComponent(u32 entityId, i32 newComponentId, ComponentIndices componentIndex);
    void UpdateComponentsWithTransform();

    void SystemPhysicsUpdate(float dt);
    void SystemScreenBounceUpdate();
    void SystemSpriteDraw();
};


#endif //SUNSET_REVERIES_ECSMANAGER_H
