//
// Created by gaetz on 05/11/2022.
//

#ifndef GAEMI_ENGINE_H
#define GAEMI_ENGINE_H

#include <string>

class Engine {
public:
    __declspec(dllexport) void Start(int windowWidth, int windowHeight, const std::string& gameName);

    std::string gameName;
};


#endif //GAEMI_ENGINE_H
