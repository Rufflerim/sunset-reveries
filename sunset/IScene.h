//
// Created by gaetz on 06/11/2022.
//

#ifndef SUNSET_REVERIES_ISCENE_H
#define SUNSET_REVERIES_ISCENE_H

class Renderer;

class IScene {
public:
    virtual ~IScene() = default;

    virtual void Load() = 0;
    virtual void Update(float dt) = 0;
    virtual void Draw(const Renderer& renderer) = 0;
    virtual void Unload() = 0;

    bool getLocking() const { return isLocking; }
    bool getTransparent() const { return isTransparent; }

protected:
    bool isLocking { true };
    bool isTransparent { false };
};

#endif //SUNSET_REVERIES_ISCENE_H
