//
// Created by gaetz on 06/11/2022.
//

#include "PlatformUtils.hpp"
#include "LogLevel.hpp"

#ifdef GPLATFORM_WINDOWS
#include <array>
#include <windows.h>
#else
#include <iostream>
#endif


void utils::ConsoleWrite(const str& message, LogLevel level) {

#ifdef GPLATFORM_WINDOWS
    // Colors for Fatal, Error, Warning, Info, Debug, Trace
    std::array<u8, 6> levels { 64, 4, 6, 2, 1, 8 };

    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(consoleHandle, levels[static_cast<i32>(level)]);
    OutputDebugStringA(message.c_str());
    LPDWORD numberWritten {};
    WriteConsoleA(consoleHandle, message.c_str(), static_cast<DWORD>(message.length()), numberWritten, nullptr);
    // Get back to white for other logging systems
    SetConsoleTextAttribute(consoleHandle, 7);
#else
    str colorStart { "\033[" };
    switch (level) {
        case LogLevel::Fatal:
            colorStart += "41;30m";
            break;
        case LogLevel::Error:
            colorStart += "0;31m";
            break;
        case LogLevel::Warning:
            colorStart += "0;33m";
            break;
        case LogLevel::Info:
            colorStart += "0;34m";
            break;
        case LogLevel::Debug:
            colorStart += "0;36m";
            break;
        case LogLevel::Trace:
            colorStart += "0;37m";
            break;
    }

    std::cout << colorStart << message << "\033[0m";
#endif
}