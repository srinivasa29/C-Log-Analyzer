#ifndef LOG_ANALYZER_H
#define LOG_ANALYZER_H

#include "LogEntry.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

/**
 * Class responsible for loading, parsing, storing, and analyzing log files.
 */
class LogAnalyzer {
private:
    std::string filePath;
    std::vector<LogEntry> entries;
    std::unordered_map<LogLevel, size_t> levelCounts;
    std::unordered_map<std::string, size_t> errorFrequency;
    bool loaded;

    // Helper method to reset analyzer state
    void reset();

public:
    LogAnalyzer();

    // Loads and analyzes a log file from the given path.
    // Throws std::runtime_error on file IO failures.
    bool loadLogFile(const std::string& path);

    // Queries / Getters
    size_t getTotalLines() const;
    size_t getCountByLevel(LogLevel level) const;
    const std::vector<LogEntry>& getEntries() const;
    bool isLoaded() const;
    const std::string& getLoadedFilePath() const;

    // Advanced analytics
    size_t getUniqueErrorCount() const;

    // Normalizes error messages by masking dynamic parameters (user_id, tx_id, etc.)
    static std::string normalizeErrorMessage(const std::string& message);

    // Returns top N error messages sorted by frequency descending
    std::vector<std::pair<std::string, size_t>> getTopErrors(size_t topN = 5) const;

    // Keyword search across log messages and raw lines (case-insensitive)
    std::vector<LogEntry> searchLogs(const std::string& keyword) const;
};

#endif // LOG_ANALYZER_H
