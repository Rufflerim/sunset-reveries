//
// Created by gaetz on 05/11/2022.
//

#include "Log.h"
#include <iostream>
#include "PlatformUtils.h"

std::ofstream Log::file;

Log::Log() {
    file.open(GAME_LOG_FILE, std::fstream::app);
}

Log::~Log() {
    stream << std::endl;

    file << stream.str();
    utils::ConsoleWrite(stream.str(), logLevel);

    stream.clear();
    file.close();
}

std::ostringstream& Log::Get(LogLevel level) {
    logLevel = level;
    stream <<  GetLabel(level) << ": [" << GetDate().data() << "] \t";
    return stream;
}

void Log::Restart() {
    file.open(GAME_LOG_FILE, std::fstream::trunc);
    file.close();
}

str Log::GetLabel(LogLevel level) {
    str label;
    switch (level) {
        case LogLevel::Fatal:
            label = "FATAL";
            break;
        case LogLevel::Error:
            label = "ERROR";
            break;
        case LogLevel::Warning:
            label = "WARNING";
            break;
        case LogLevel::Info:
            label = "INFO";
            break;
        case LogLevel::Debug:
            label = "DEBUG";
            break;
        case LogLevel::Trace:
            label = "TRACE";
            break;
    }
    return label;
}

std::array<char, 19> Log::GetDate() {
#ifdef GPLATFORM_WINDOWS
    time_t now;
    std::array <char, 19> date {};
    struct tm timeInfo {};
    time(&now);
    localtime_s(&timeInfo, &now);
    strftime(date.data(), date.size(), "%y-%m-%d %H:%M:%S", &timeInfo);
#else
    std::array <char, 19> date {};
#endif
    return date;
}
