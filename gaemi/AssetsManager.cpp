//
// Created by gaetz on 06/11/2022.
//

#include "AssetsManager.h"

unordered_map<str, Texture> AssetsManager::textures {};

void AssetsManager::LoadTexture(const str& name, const str& path) {
    Texture texture = ::LoadTexture(path.c_str());
    textures.emplace(name, texture);
}

Texture AssetsManager::GetTexture(const str& name) {
    return textures.at(name);
}
