//
// Created by gaetz on 06/11/2022.
//

#ifndef GAEMI_ASSETS_MANAGER_HPP
#define GAEMI_ASSETS_MANAGER_HPP

#include <unordered_map>
#include "Defines.hpp"
#include "raylib.h"

using std::unordered_map;

class AssetsManager {
public:
    static GAPI void LoadTexture(const str& name, const str& path, i32 sceneId);
    static GAPI Texture GetTexture(const str& name);

    static GAPI void UnloadSceneTextures(i32 sceneId);

private:
    static unordered_map<str, Texture> textures;
    static unordered_map<i32, str> sceneLoadedTextures;

};


#endif //GAEMI_ASSETS_MANAGER_HPP
