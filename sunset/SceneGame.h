//
// Created by gaetz on 06/11/2022.
//

#ifndef SUNSET_REVERIES_SCENEGAME_H
#define SUNSET_REVERIES_SCENEGAME_H

#include "IScene.h"
#include "Entity.h"
#include "raylib.h"
#include "Components.h"

#include <vector>
#include <array>

using std::vector;
using std::array;

struct Collision2D {
    Collision2D(u32 entityId, Rectangle currentBox, Vector2 velocity,
                u32 otherId, Rectangle otherCurrentBox, Vector2 otherVelocity):
            entityId { entityId}, currentBox { currentBox }, velocity { velocity },
            otherId { otherId }, otherCurrentBox { otherCurrentBox }, otherVelocity { otherVelocity }
    {}

    u32 entityId;
    Rectangle currentBox;
    Vector2 velocity;
    u32 otherId;
    Rectangle otherCurrentBox;
    Vector2 otherVelocity;
};

class SceneGame : public IScene {
public:
    void Load() override;
    void Update(float dt) override;
    void Draw() override;
    void Unload() override;

private:
    Texture2D texture;
    Texture2D playerTexture;

    vector<Entity> entities;
    vector<Transform2D> transforms;
    vector<Sprite> sprites;
    vector<Rigidbody2D> bodies;

    u32 CreateEntity();
    u32 CreateRandomBouncingEntity();

    void CreateTransform2DComponent(u32 entityId);
    void CreateSpriteComponent(u32 entityId, const str& texName);
    void CreateRigidbody2DComponent(u32 entityId, const Rectangle& box);
    void UpdateEntityWithComponent(u32 entityId, i32 newComponentId, ComponentIndices componentIndex);

    void SystemSpriteDraw();
    void SystemPhysicsUpdate(float dt);
    void SystemScreenBounceUpdate();

    Transform2D& GetTransform2DComponent(u32 entityId);
    Sprite& GetSpriteComponent(u32 entityId);
    Rigidbody2D& GetRigidbody2DComponent(u32 entityId);
};


#endif //SUNSET_REVERIES_SCENEGAME_H
