#include "LogEntry.h"
#include <algorithm>
#include <cctype>

std::string logLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::INFO:    return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR:   return "ERROR";
        case LogLevel::DEBUG:   return "DEBUG";
        default:                return "UNKNOWN";
    }
}

LogLevel stringToLogLevel(const std::string& levelStr) {
    std::string upperStr = levelStr;
    std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(),
                   [](unsigned char c) { return static_cast<char>(std::toupper(c)); });

    if (upperStr == "INFO") return LogLevel::INFO;
    if (upperStr == "WARNING" || upperStr == "WARN") return LogLevel::WARNING;
    if (upperStr == "ERROR" || upperStr == "ERR") return LogLevel::ERROR;
    if (upperStr == "DEBUG") return LogLevel::DEBUG;
    return LogLevel::UNKNOWN;
}

LogEntry::LogEntry()
    : timestamp(""), level(LogLevel::UNKNOWN), message(""), rawLine("") {}

LogEntry::LogEntry(std::string timestamp, LogLevel level, std::string message, std::string rawLine)
    : timestamp(std::move(timestamp)),
      level(level),
      message(std::move(message)),
      rawLine(std::move(rawLine)) {}

const std::string& LogEntry::getTimestamp() const {
    return timestamp;
}

LogLevel LogEntry::getLevel() const {
    return level;
}

std::string LogEntry::getLevelString() const {
    return logLevelToString(level);
}

const std::string& LogEntry::getMessage() const {
    return message;
}

const std::string& LogEntry::getRawLine() const {
    return rawLine;
}

LogEntry LogEntry::parseLine(const std::string& line) {
    if (line.empty()) {
        return LogEntry("", LogLevel::UNKNOWN, "", line);
    }

    std::string timestampStr = "";
    LogLevel parsedLevel = LogLevel::UNKNOWN;
    std::string messageStr = "";

    // Parse typical format: "YYYY-MM-DD HH:MM:SS [LEVEL] Message payload..."
    size_t openBracket = line.find('[');
    size_t closeBracket = line.find(']', openBracket);

    if (openBracket != std::string::npos && closeBracket != std::string::npos && closeBracket > openBracket) {
        // Extract timestamp (everything before '[' trimmed)
        timestampStr = line.substr(0, openBracket);
        // Trim trailing spaces from timestampStr
        size_t lastNonSpace = timestampStr.find_last_not_of(" \t");
        if (lastNonSpace != std::string::npos) {
            timestampStr = timestampStr.substr(0, lastNonSpace + 1);
        } else {
            timestampStr = "";
        }

        // Extract level string inside brackets
        std::string levelSub = line.substr(openBracket + 1, closeBracket - openBracket - 1);
        parsedLevel = stringToLogLevel(levelSub);

        // Extract message (everything after ']')
        if (closeBracket + 1 < line.length()) {
            messageStr = line.substr(closeBracket + 1);
            // Trim leading spaces from message
            size_t firstNonSpace = messageStr.find_first_not_of(" \t");
            if (firstNonSpace != std::string::npos) {
                messageStr = messageStr.substr(firstNonSpace);
            } else {
                messageStr = "";
            }
        }
    } else {
        // Alternative format without brackets e.g. "2026-09-30 10:15:20 INFO User logged in"
        // Look for keywords
        std::string upperLine = line;
        std::transform(upperLine.begin(), upperLine.end(), upperLine.begin(),
                       [](unsigned char c) { return static_cast<char>(std::toupper(c)); });

        if (upperLine.find("ERROR") != std::string::npos || upperLine.find("ERR") != std::string::npos) {
            parsedLevel = LogLevel::ERROR;
        } else if (upperLine.find("WARNING") != std::string::npos || upperLine.find("WARN") != std::string::npos) {
            parsedLevel = LogLevel::WARNING;
        } else if (upperLine.find("INFO") != std::string::npos) {
            parsedLevel = LogLevel::INFO;
        } else if (upperLine.find("DEBUG") != std::string::npos) {
            parsedLevel = LogLevel::DEBUG;
        }

        messageStr = line;
    }

    return LogEntry(timestampStr, parsedLevel, messageStr, line);
}
