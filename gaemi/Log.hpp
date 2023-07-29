//
// Created by gaetz on 05/11/2022.
//

#ifndef GAEMI_LOG_HPP
#define GAEMI_LOG_HPP

#include "Defines.hpp"
#include "raylib.h"
#include <sstream>
#include <fstream>
#include "LogLevel.hpp"

#include <unordered_map>
#include <functional>

using std::unordered_map;

class Log {
public:
    GAPI Log();
    GAPI ~Log();
    const static str GAME_LOG_FILE;

    Log(const Log&) = delete;
    Log& operator=(const Log&) = delete;

    GAPI std::ostringstream& Get(LogLevel level = LogLevel::Info);
    GAPI static void Restart();

    template <class T, class U>
    GAPI static str LogMap(const str& name, const unordered_map<T, U>& map, std::function<str(T, U)> decoder) {
        std::stringstream stream;
        stream << "Map " << name << std::endl;
        for (const auto& pair : map) {
            stream << decoder(pair.first, pair.second) << std::endl;
        }
        return stream.str();
    }

private:
    std::ostringstream stream;
    static std::ofstream file;
    LogLevel logLevel { LogLevel::Trace };

    static str GetLabel(LogLevel level);
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


#endif //GAEMI_LOG_HPP
