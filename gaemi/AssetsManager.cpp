//
// Created by gaetz on 06/11/2022.
//

#include "AssetsManager.h"

unordered_map<str, Texture> AssetsManager::textures {};
unordered_map<i32, str> AssetsManager::sceneLoadedTextures {};

void AssetsManager::LoadTexture(const str& name, const str& path, i32 sceneId) {
    Texture texture = ::LoadTexture(path.c_str());
    textures.emplace(name, texture);
    sceneLoadedTextures.emplace(sceneId, name);
}

const Texture AssetsManager::GetTexture(const str& name) {
    return textures.at(name);
}

void AssetsManager::UnloadSceneTextures(i32 sceneId) {
    for (auto& textureId : sceneLoadedTextures) {
        if (textureId.first == sceneId) {
            UnloadTexture(GetTexture(textureId.second));
            textures.erase(textureId.second);
        }
    }
}
