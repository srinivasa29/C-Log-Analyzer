#include "LogAnalyzer.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <regex>

LogAnalyzer::LogAnalyzer() : filePath(""), loaded(false) {
    reset();
}

void LogAnalyzer::reset() {
    entries.clear();
    levelCounts.clear();
    errorFrequency.clear();
    levelCounts[LogLevel::INFO] = 0;
    levelCounts[LogLevel::WARNING] = 0;
    levelCounts[LogLevel::ERROR] = 0;
    levelCounts[LogLevel::DEBUG] = 0;
    levelCounts[LogLevel::UNKNOWN] = 0;
    loaded = false;
    filePath = "";
}

std::string LogAnalyzer::normalizeErrorMessage(const std::string& message) {
    std::string result = message;

    // Group user IDs (e.g., user_id: 2091, user_id=1042)
    result = std::regex_replace(result, std::regex(R"(user_id[:=]\s*\w+)", std::regex_constants::icase), "user_id: <ID>");

    // Group transaction IDs (e.g., tx_99218, tx_abc123)
    result = std::regex_replace(result, std::regex(R"(tx_[a-zA-Z0-9]+)", std::regex_constants::icase), "tx_<ID>");

    // Group generic identifiers (e.g., id: 12345, session: abcd123)
    result = std::regex_replace(result, std::regex(R"(\b(id|uid|session|token)[:=]\s*\w+)", std::regex_constants::icase), "$1: <ID>");

    return result;
}

bool LogAnalyzer::loadLogFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: '" + path + "'. File does not exist or access is denied.");
    }

    reset();
    filePath = path;

    std::string line;
    while (std::getline(file, line)) {
        // Strip trailing carriage return if reading Windows CRLF line endings
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        // Ignore empty lines or lines containing only whitespace
        if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }

        LogEntry entry = LogEntry::parseLine(line);
        entries.push_back(entry);
        levelCounts[entry.getLevel()]++;

        // Track frequency of ERROR messages using normalized patterns
        if (entry.getLevel() == LogLevel::ERROR) {
            std::string normalizedError = normalizeErrorMessage(entry.getMessage());
            errorFrequency[normalizedError]++;
        }
    }

    file.close();
    loaded = true;
    return true;
}

size_t LogAnalyzer::getTotalLines() const {
    return entries.size();
}

size_t LogAnalyzer::getCountByLevel(LogLevel level) const {
    auto it = levelCounts.find(level);
    if (it != levelCounts.end()) {
        return it->second;
    }
    return 0;
}

size_t LogAnalyzer::getUniqueErrorCount() const {
    return errorFrequency.size();
}

const std::vector<LogEntry>& LogAnalyzer::getEntries() const {
    return entries;
}

bool LogAnalyzer::isLoaded() const {
    return loaded;
}

const std::string& LogAnalyzer::getLoadedFilePath() const {
    return filePath;
}

std::vector<std::pair<std::string, size_t>> LogAnalyzer::getTopErrors(size_t topN) const {
    // Copy error map into a vector for sorting
    std::vector<std::pair<std::string, size_t>> sortedErrors(
        errorFrequency.begin(), errorFrequency.end()
    );

    // Sort descending by error count
    std::sort(sortedErrors.begin(), sortedErrors.end(),
              [](const std::pair<std::string, size_t>& a, const std::pair<std::string, size_t>& b) {
                  return a.second > b.second; // Primary: Frequency descending
              });

    if (sortedErrors.size() > topN) {
        sortedErrors.resize(topN);
    }

    return sortedErrors;
}

std::vector<LogEntry> LogAnalyzer::searchLogs(const std::string& keyword) const {
    std::vector<LogEntry> results;
    if (keyword.empty()) {
        return results;
    }

    // Convert search keyword to lowercase for case-insensitive search
    std::string lowerKeyword = keyword;
    std::transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

    for (const auto& entry : entries) {
        std::string lowerRaw = entry.getRawLine();
        std::transform(lowerRaw.begin(), lowerRaw.end(), lowerRaw.begin(),
                       [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

        if (lowerRaw.find(lowerKeyword) != std::string::npos) {
            results.push_back(entry);
        }
    }

    return results;
}
