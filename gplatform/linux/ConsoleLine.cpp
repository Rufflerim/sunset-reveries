//
// Created by gaetz on 06/11/2022.
//

#include "../ConsoleLine.hpp"
#include <iostream>

namespace gplatform {

    void ConsoleWrite(const str &message, i32 level) {
        str colorStart{"\033["};
        switch (level) {
            case 0:
                colorStart += "41;30m";
                break;
            case 1:
                colorStart += "0;31m";
                break;
            case 2:
                colorStart += "0;33m";
                break;
            case 3:
                colorStart += "0;34m";
                break;
            case 4:
                colorStart += "0;36m";
                break;
            case 5:
                colorStart += "0;37m";
                break;
        }

        std::cout << colorStart << message << "\033[0m";
    }

}