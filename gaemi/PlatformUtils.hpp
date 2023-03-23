//
// Created by gaetz on 06/11/2022.
//

#ifndef SUNSET_REVERIES_PLATFORM_UTILS_HPP
#define SUNSET_REVERIES_PLATFORM_UTILS_HPP

#include "Defines.hpp"

enum class LogLevel;

namespace utils {
    void ConsoleWrite(const str& message, LogLevel level);
}

#endif //SUNSET_REVERIES_PLATFORM_UTILS_HPP
