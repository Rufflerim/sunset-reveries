//
// Created by gaetz on 05/11/2022.
//

#ifndef GAEMI_WINDOW_H
#define GAEMI_WINDOW_H

#include <string>
#include "Defines.h"

class Window {
public:
    Window(i32 width, i32 height, str title);
    ~Window();

    bool ShouldClose();

private:
    i32 width;
    i32 height;
    str title;

};


#endif //GAEMI_WINDOW_H
