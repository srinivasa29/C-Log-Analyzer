#include "ReportGenerator.h"
#include <iostream>
#include <fstream>
#include <iomanip>

void ReportGenerator::printSummary(const LogAnalyzer& analyzer) {
    if (!analyzer.isLoaded()) {
        std::cout << "\n[!] No log file has been loaded yet. Please select option 1 to analyze a log file.\n";
        return;
    }

    size_t total = analyzer.getTotalLines();
    size_t infoCount = analyzer.getCountByLevel(LogLevel::INFO);
    size_t debugCount = analyzer.getCountByLevel(LogLevel::DEBUG);
    size_t warnCount = analyzer.getCountByLevel(LogLevel::WARNING);
    size_t errCount = analyzer.getCountByLevel(LogLevel::ERROR);
    size_t unknownCount = analyzer.getCountByLevel(LogLevel::UNKNOWN);

    auto calcPct = [total](size_t count) -> double {
        return total > 0 ? (static_cast<double>(count) * 100.0 / total) : 0.0;
    };

    std::cout << "\n========================================\n";
    std::cout << "          LOG ANALYZER SUMMARY          \n";
    std::cout << "========================================\n";
    std::cout << " File Path    : " << analyzer.getLoadedFilePath() << "\n";
    std::cout << " Total Lines  : " << total << "\n";
    std::cout << "----------------------------------------\n";
    std::cout << std::fixed << std::setprecision(1);
    std::cout << " INFO         : " << std::setw(6) << infoCount    << "  (" << calcPct(infoCount)    << "%)\n";
    std::cout << " DEBUG        : " << std::setw(6) << debugCount   << "  (" << calcPct(debugCount)   << "%)\n";
    std::cout << " WARNING      : " << std::setw(6) << warnCount    << "  (" << calcPct(warnCount)    << "%)\n";
    std::cout << " ERROR        : " << std::setw(6) << errCount     << "  (" << calcPct(errCount)     << "%)\n";
    if (unknownCount > 0) {
        std::cout << " UNKNOWN      : " << std::setw(6) << unknownCount << "  (" << calcPct(unknownCount) << "%)\n";
    }
    std::cout << "----------------------------------------\n";
    std::cout << " Unique Errors: " << analyzer.getUniqueErrorCount() << "\n";
    std::cout << "========================================\n";
}

void ReportGenerator::printTopErrors(const LogAnalyzer& analyzer, size_t topN) {
    if (!analyzer.isLoaded()) {
        std::cout << "\n[!] No log file has been loaded yet.\n";
        return;
    }

    auto topErrors = analyzer.getTopErrors(topN);

    std::cout << "\n================================================================================\n";
    std::cout << "                         TOP ERRORS (GROUPED PATTERNS)                          \n";
    std::cout << "================================================================================\n";
    std::cout << " Total Errors Logged    : " << analyzer.getCountByLevel(LogLevel::ERROR) << "\n";
    std::cout << " Unique Error Signatures: " << analyzer.getUniqueErrorCount() << "\n";
    std::cout << "--------------------------------------------------------------------------------\n";

    if (topErrors.empty()) {
        std::cout << "  No ERROR messages found in the log file!\n";
    } else {
        std::cout << " Rank   Count   Error Signature (Normalized Pattern)\n";
        std::cout << "--------------------------------------------------------------------------------\n";
        size_t rank = 1;
        for (const auto& errPair : topErrors) {
            std::cout << " [" << std::setw(2) << rank++ << "]   "
                      << std::setw(4) << errPair.second << "x   "
                      << errPair.first << "\n";
        }
    }
    std::cout << "================================================================================\n";
}

void ReportGenerator::printSearchResults(const std::vector<LogEntry>& results, const std::string& keyword) {
    std::cout << "\n========================================\n";
    std::cout << " SEARCH RESULTS FOR: \"" << keyword << "\"\n";
    std::cout << "========================================\n";

    if (results.empty()) {
        std::cout << " No matching log lines found.\n";
    } else {
        std::cout << " Found " << results.size() << " matching line(s):\n\n";
        size_t index = 1;
        for (const auto& entry : results) {
            std::cout << " [" << index++ << "] " << entry.getRawLine() << "\n";
        }
    }
    std::cout << "========================================\n";
}

bool ReportGenerator::exportReport(const LogAnalyzer& analyzer, const std::string& outputPath) {
    if (!analyzer.isLoaded()) {
        std::cout << "\n[!] Cannot export report. No log file loaded.\n";
        return false;
    }

    std::ofstream outFile(outputPath);
    if (!outFile.is_open()) {
        // Fallback to local report.txt if subfolder path fails
        outFile.open("report.txt");
        if (!outFile.is_open()) {
            std::cerr << "\n[!] Error: Failed to open output file for exporting report.\n";
            return false;
        }
    }

    size_t total = analyzer.getTotalLines();
    size_t infoCount = analyzer.getCountByLevel(LogLevel::INFO);
    size_t debugCount = analyzer.getCountByLevel(LogLevel::DEBUG);
    size_t warnCount = analyzer.getCountByLevel(LogLevel::WARNING);
    size_t errCount = analyzer.getCountByLevel(LogLevel::ERROR);
    size_t unknownCount = analyzer.getCountByLevel(LogLevel::UNKNOWN);

    auto calcPct = [total](size_t count) -> double {
        return total > 0 ? (static_cast<double>(count) * 100.0 / total) : 0.0;
    };

    outFile << "================================================================================\n";
    outFile << "                   C++ LOG ANALYZER & ERROR MONITORING REPORT                   \n";
    outFile << "================================================================================\n";
    outFile << " Analyzed File          : " << analyzer.getLoadedFilePath() << "\n";
    outFile << " Total Lines Processed  : " << total << "\n";
    outFile << " Health Status          : " << (errCount > 0 ? "ACTION REQUIRED (Errors Detected)" : "HEALTHY (Zero Errors)") << "\n";
    outFile << "--------------------------------------------------------------------------------\n";
    outFile << " LOG LEVEL BREAKDOWN:\n";
    outFile << std::fixed << std::setprecision(1);
    outFile << "   INFO                 : " << std::setw(6) << infoCount    << "  (" << calcPct(infoCount)    << "%)\n";
    outFile << "   DEBUG                : " << std::setw(6) << debugCount   << "  (" << calcPct(debugCount)   << "%)\n";
    outFile << "   WARNING              : " << std::setw(6) << warnCount    << "  (" << calcPct(warnCount)    << "%)\n";
    outFile << "   ERROR                : " << std::setw(6) << errCount     << "  (" << calcPct(errCount)     << "%)\n";
    if (unknownCount > 0) {
        outFile << "   UNKNOWN              : " << std::setw(6) << unknownCount << "  (" << calcPct(unknownCount) << "%)\n";
    }
    outFile << "--------------------------------------------------------------------------------\n";
    outFile << " ERROR METRICS:\n";
    outFile << "   Total Errors         : " << errCount << "\n";
    outFile << "   Unique Error Patterns: " << analyzer.getUniqueErrorCount() << "\n";
    outFile << "================================================================================\n\n";

    outFile << "TOP FREQUENT ERROR PATTERNS (GROUPED BY SIGNATURE):\n";
    outFile << "--------------------------------------------------------------------------------\n";
    auto topErrors = analyzer.getTopErrors(10);
    if (topErrors.empty()) {
        outFile << " No ERROR messages recorded in this log.\n";
    } else {
        outFile << " Rank   Count   Error Signature (Normalized)\n";
        outFile << "--------------------------------------------------------------------------------\n";
        size_t rank = 1;
        for (const auto& errPair : topErrors) {
            outFile << " [" << std::setw(2) << rank++ << "]   "
                    << std::setw(4) << errPair.second << "x   "
                    << errPair.first << "\n";
        }
    }
    outFile << "================================================================================\n";

    outFile.close();
    return true;
}
