//
// Created by gaetz on 05/11/2022.
//

#ifndef GAEMI_LOG_H
#define GAEMI_LOG_H

#include "Defines.h"
#include "raylib.h"
#include <sstream>
#include <fstream>
#include <array>
#include "LogLevel.h"

const str GAME_LOG_FILE = "Reveries.log";

class Log {
public:
    GAPI Log();
    GAPI ~Log();

    Log(const Log&) = delete;
    Log& operator=(const Log&) = delete;

    GAPI std::ostringstream& Get(LogLevel level = LogLevel::Info);
    GAPI static void Restart();

private:
    std::ostringstream stream;
    static std::ofstream file;
    LogLevel logLevel { LogLevel::Trace };

    str GetLabel(LogLevel level);
    std::array<char, 19> GetDate();
};

#ifdef GDEBUG
    constexpr i32 MAX_LOG_LEVEL = static_cast<i32>(LogLevel::Trace);
#else
    constexpr i32 MAX_LOG_LEVEL = static_cast<i32>(LogLevel::Error);
#endif

#define LOG(level)                                  \
    if (static_cast<i32>(level) > MAX_LOG_LEVEL)    \
        ;                                           \
    else                                            \
        Log().Get(level)


#endif //GAEMI_LOG_H
