//
// Created by gaetz on 06/11/2022.
//

#include "AssetsManager.hpp"
#include <fstream>
#include <sstream>
#include <vector>

unordered_map<str, Texture> AssetsManager::textures {};
unordered_map<i32, str> AssetsManager::sceneLoadedTextures {};
unordered_map<str, f32> AssetsManager::data {};

void AssetsManager::LoadTexture(const str& name, const str& path, i32 sceneId) {
    Texture texture = ::LoadTexture(path.c_str());
    textures.emplace(name, texture);
    sceneLoadedTextures.emplace(sceneId, name);
}

Texture AssetsManager::GetTexture(const str& name) {
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

void AssetsManager::LoadData() {
    const unordered_map<str, f32> playerData { std::move(ReadFile("assets/data/player.data")) };
    data.insert(playerData.begin(), playerData.end());
    const unordered_map<str, f32> physicsData { std::move(ReadFile("assets/data/physics.data")) };
    data.insert(physicsData.begin(), physicsData.end());
}

f32 AssetsManager::GetData(const str &name) {
    return data.at(name);
}

unordered_map<str, f32> AssetsManager::ReadFile(const str &path) {
    unordered_map<str, f32> fileData;
    std::ifstream file { path };
    std::vector<str> lines {};
    str fileLine;
    while (std::getline(file, fileLine)) {
        lines.push_back(fileLine);
    }
    file.close();

    for (auto& line : lines) {
        std::istringstream lineStream { line };
        std::vector<str> words;
        str word;
        while (std::getline(lineStream, word, ' ')) {
            words.push_back(word);
        }
        fileData.emplace(words[0], std::stof(words[1]));
    }
    return fileData;
}
