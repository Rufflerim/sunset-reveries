//
// Created by gaetz on 06/11/2022.
//

#include "PlatformUtils.h"
#include "LogLevel.h"
#include <iostream>

#ifdef GPLATFORM_WINDOWS
#include <array>
#include <windows.h>
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
    std::cout << message;
#endif
}