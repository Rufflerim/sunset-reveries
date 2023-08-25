//
// Created by gaetz on 30/07/2023.
//

#include "File.hpp"

#include <sstream>
#include <fstream>

unordered_map<str, f32> File::ReadFile(FileType fileType, const str &filename) {
    unordered_map<str, f32> fileData;
    str path = FileTypePath(fileType) + filename;
    std::ifstream file { path };
    vector<str> lines {};
    str fileLine;
    while (std::getline(file, fileLine)) {
        lines.push_back(fileLine);
    }
    file.close();

    for (auto& line : lines) {
        std::istringstream lineStream { line };
        vector<str> words;
        str word;
        while (std::getline(lineStream, word, ' ')) {
            words.push_back(word);
        }
        fileData.emplace(words[0], std::stof(words[1]));
    }
    return fileData;
}