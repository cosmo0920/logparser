#pragma once

#include <iostream>
#include <array>
#include <algorithm>

enum class LogLevel {
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Fatal
};

const std::array<LogLevel, 6> logLevels{{
    LogLevel::Trace,
    LogLevel::Debug,
    LogLevel::Info,
    LogLevel::Warn,
    LogLevel::Error,
    LogLevel::Fatal}};

const std::array<std::string, 6> logLevelStrings{{
    "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"}};

LogLevel mkLogLevel(std::string str) {
    auto count = count_if(
        logLevelStrings.begin(),
        logLevelStrings.end(),
        [&str](std::string x){ return str == x; });
    return logLevels[count];
}

std::ostream& operator<<(std::ostream& os, LogLevel level) {
    auto count = std::count_if(
        logLevels.begin(),
        logLevels.end(),
        [&level](LogLevel l){ return l == level; });
    return os << logLevelStrings[count];
}
