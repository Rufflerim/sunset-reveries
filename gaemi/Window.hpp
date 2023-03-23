//
// Created by gaetz on 05/11/2022.
//

#ifndef GAEMI_WINDOW_HPP
#define GAEMI_WINDOW_HPP

#include <string>
#include "Defines.hpp"

class Window {
public:
    Window(i32 width, i32 height, str title);
    ~Window();

    static bool ShouldClose();

private:
    i32 width;
    i32 height;
    str title;
};


#endif //GAEMI_WINDOW_HPP
