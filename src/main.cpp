#include "LogAnalyzer.h"
#include "ReportGenerator.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>

void displayMenu() {
    std::cout << "\n========================================\n";
    std::cout << "  C++ LOG ANALYZER & MONITORING TOOL    \n";
    std::cout << "========================================\n";
    std::cout << " 1. Analyze Log File\n";
    std::cout << " 2. Show Statistics\n";
    std::cout << " 3. Show Top Errors\n";
    std::cout << " 4. Search Logs\n";
    std::cout << " 5. Export Report\n";
    std::cout << " 6. Exit\n";
    std::cout << "========================================\n";
    std::cout << " Select an option (1-6): ";
}

int main() {
    LogAnalyzer analyzer;
    std::string defaultPath = "logs/sample.log";

    // Auto-load sample log if present on startup for smooth UX
    std::ifstream sampleCheck(defaultPath);
    if (sampleCheck.is_open()) {
        sampleCheck.close();
        try {
            analyzer.loadLogFile(defaultPath);
            std::cout << "[+] Automatically loaded default sample log file: '" << defaultPath << "'\n";
        } catch (const std::exception& ex) {
            // Ignore auto-load failure silently, user can load via menu
        }
    }

    std::string inputLine;
    while (true) {
        displayMenu();
        if (!std::getline(std::cin, inputLine)) {
            // Reached EOF or stream closed
            std::cout << "\nInput stream closed. Exiting Log Analyzer. Goodbye!\n";
            break;
        }

        // Trim leading and trailing whitespace
        size_t first = inputLine.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) {
            std::cout << "\n[!] Empty input! Please enter a number between 1 and 6.\n";
            continue;
        }
        size_t last = inputLine.find_last_not_of(" \t\r\n");
        std::string choiceStr = inputLine.substr(first, (last - first + 1));

        int choice = 0;
        try {
            choice = std::stoi(choiceStr);
        } catch (...) {
            std::cout << "\n[!] Invalid input '" << choiceStr << "'! Please enter a number between 1 and 6.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                std::string filePath;
                std::cout << "\nEnter log file path (Press Enter for default: '" << defaultPath << "'): ";
                std::getline(std::cin, filePath);
                if (filePath.empty()) {
                    filePath = defaultPath;
                }

                try {
                    if (analyzer.loadLogFile(filePath)) {
                        std::cout << "\n[+] Log file successfully analyzed!\n";
                        ReportGenerator::printSummary(analyzer);
                    }
                } catch (const std::exception& ex) {
                    std::cerr << "\n[!] Error loading file: " << ex.what() << "\n";
                }
                break;
            }
            case 2: {
                ReportGenerator::printSummary(analyzer);
                break;
            }
            case 3: {
                if (!analyzer.isLoaded()) {
                    std::cout << "\n[!] Please load a log file first (Option 1).\n";
                    break;
                }
                size_t topN = 5;
                std::cout << "Enter number of top errors to display (default 5): ";
                std::string inputN;
                std::getline(std::cin, inputN);
                if (!inputN.empty()) {
                    try {
                        topN = std::stoul(inputN);
                    } catch (...) {
                        std::cout << "[!] Invalid number. Displaying top 5 by default.\n";
                        topN = 5;
                    }
                }
                ReportGenerator::printTopErrors(analyzer, topN);
                break;
            }
            case 4: {
                if (!analyzer.isLoaded()) {
                    std::cout << "\n[!] Please load a log file first (Option 1).\n";
                    break;
                }
                std::string keyword;
                std::cout << "\nEnter keyword to search: ";
                std::getline(std::cin, keyword);
                if (keyword.empty()) {
                    std::cout << "[!] Search keyword cannot be empty.\n";
                } else {
                    auto results = analyzer.searchLogs(keyword);
                    ReportGenerator::printSearchResults(results, keyword);
                }
                break;
            }
            case 5: {
                if (!analyzer.isLoaded()) {
                    std::cout << "\n[!] Please load a log file first (Option 1).\n";
                    break;
                }
                std::string exportPath = "reports/report.txt";
                std::cout << "\nEnter output report path (Press Enter for default: 'reports/report.txt'): ";
                std::string customPath;
                std::getline(std::cin, customPath);
                if (!customPath.empty()) {
                    exportPath = customPath;
                }

                if (ReportGenerator::exportReport(analyzer, exportPath)) {
                    std::cout << "\n[+] Report successfully exported to: " << exportPath << "\n";
                } else {
                    std::cout << "\n[!] Failed to export report.\n";
                }
                break;
            }
            case 6: {
                std::cout << "\nExiting Log Analyzer. Goodbye!\n";
                return 0;
            }
            default: {
                std::cout << "\n[!] Invalid choice! Please select an option between 1 and 6.\n";
                break;
            }
        }
    }

    return 0;
}
