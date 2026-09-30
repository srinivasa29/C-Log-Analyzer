#ifndef LOG_ENTRY_H
#define LOG_ENTRY_H

#include <string>

// Enum representing the supported log severity levels
enum class LogLevel {
    INFO,
    WARNING,
    ERROR,
    DEBUG,
    UNKNOWN
};

// Converts LogLevel enum to string representation
std::string logLevelToString(LogLevel level);

// Converts string representation to LogLevel enum
LogLevel stringToLogLevel(const std::string& levelStr);

/**
 * Class representing a single parsed log line.
 * Encapsulates timestamp, log level, message payload, and the original raw log line.
 */
class LogEntry {
private:
    std::string timestamp;
    LogLevel level;
    std::string message;
    std::string rawLine;

public:
    // Constructors
    LogEntry();
    LogEntry(std::string timestamp, LogLevel level, std::string message, std::string rawLine);

    // Getters (const-qualified for read-only access)
    const std::string& getTimestamp() const;
    LogLevel getLevel() const;
    std::string getLevelString() const;
    const std::string& getMessage() const;
    const std::string& getRawLine() const;

    // Static helper method to parse a raw string line into a LogEntry object
    static LogEntry parseLine(const std::string& line);
};

#endif // LOG_ENTRY_H
