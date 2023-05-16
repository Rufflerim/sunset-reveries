//
// Created by gaetz on 06/11/2022.
//

#include "../ConsoleLine.hpp"

#include <windows.h>

void gplatform::ConsoleWrite(const str& message, i32 level) {
    // Colors for Fatal, Error, Warning, Info, Debug, Trace
    array<u8, 6> levels { 64, 4, 6, 2, 1, 8 };

    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(consoleHandle, levels[level]);
    OutputDebugStringA(message.c_str());
    LPDWORD numberWritten {};
    WriteConsoleA(consoleHandle, message.c_str(), static_cast<DWORD>(message.length()), numberWritten, nullptr);
    // Get back to white for other logging systems
    SetConsoleTextAttribute(consoleHandle, 7);
}