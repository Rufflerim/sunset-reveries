//
// Created by gaetz on 05/11/2022.
//

#include "Log.hpp"
#include <iostream>
#include "ConsoleLine.hpp"
#include "Calendar.hpp"
#include "Asserts.hpp"

const str Log::GAME_LOG_FILE = "Reveries.log";
std::ofstream Log::file;

Log::Log() {
    file.open(GAME_LOG_FILE, std::fstream::app);
}

Log::~Log() {
    stream << std::endl;

    file << stream.str();
    gplatform::ConsoleWrite(stream.str(), static_cast<i32>(logLevel));

    stream.clear();
    file.close();
}

std::ostringstream& Log::Get(LogLevel level) {
    logLevel = level;
    stream <<  GetLabel(level) << ": [" << gplatform::GetDate().data() << "] \t";
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

void ReportAssertionFailure(const str& expression, const str& message, const char* codeFile, i32 codeLine) {
    LOG(LogLevel::Fatal) << "Assertion failure: " << expression << ", message: " << message << ", in file: " << codeFile << " line: " << codeLine;
}