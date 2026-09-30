#ifndef REPORT_GENERATOR_H
#define REPORT_GENERATOR_H

#include "LogAnalyzer.h"
#include <string>

/**
 * Class responsible for displaying analysis metrics on console
 * and exporting summary reports to file output.
 */
class ReportGenerator {
public:
    // Prints a statistics summary report to standard output
    static void printSummary(const LogAnalyzer& analyzer);

    // Prints the top errors list to standard output
    static void printTopErrors(const LogAnalyzer& analyzer, size_t topN = 5);

    // Prints search results to standard output
    static void printSearchResults(const std::vector<LogEntry>& results, const std::string& keyword);

    // Exports full report to a text file. Returns true if successful.
    static bool exportReport(const LogAnalyzer& analyzer, const std::string& outputPath = "reports/report.txt");
};

#endif // REPORT_GENERATOR_H
