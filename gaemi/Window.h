//
// Created by gaetz on 05/11/2022.
//

#ifndef GAEMI_WINDOW_H
#define GAEMI_WINDOW_H

#include <string>

class Window {
public:
    Window(int width, int height, std::string title);
    ~Window();

    bool ShouldClose();

private:
    int width;
    int height;
    std::string title;

};


#endif //GAEMI_WINDOW_H
