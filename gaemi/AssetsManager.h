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
    static GAPI void LoadTexture(const str& name, const str& path);
    static GAPI Texture GetTexture(const str& name);


private:
    static unordered_map<str, Texture> textures;
};


#endif //GAEMI_ASSETSMANAGER_H
