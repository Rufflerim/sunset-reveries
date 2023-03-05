//
// Created by gaetz on 06/11/2022.
//

#ifndef SUNSET_REVERIES_PLATFORMUTILS_H
#define SUNSET_REVERIES_PLATFORMUTILS_H

#include "Defines.h"

enum class LogLevel;

namespace utils {
    void ConsoleWrite(const str& message, LogLevel level);
}

#endif //SUNSET_REVERIES_PLATFORMUTILS_H
