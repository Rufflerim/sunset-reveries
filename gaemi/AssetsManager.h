//
// Created by gaetz on 06/11/2022.
//

#ifndef GAEMI_ASSETSMANAGER_H
#define GAEMI_ASSETSMANAGER_H

#include <unordered_map>
#include "Defines.h"
#include "raylib.h"

using std::unordered_map;

class AssetsManager {
public:
    static GAPI void LoadTexture(const str& name, const str& path, i32 sceneId);
    static GAPI const Texture GetTexture(const str& name);

    static GAPI void UnloadSceneTextures(i32 sceneId);

private:
    static unordered_map<str, Texture> textures;
    static unordered_map<i32, str> sceneLoadedTextures;

};


#endif //GAEMI_ASSETSMANAGER_H
