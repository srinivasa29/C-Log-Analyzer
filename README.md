# C++ Log Analyzer & Error Monitoring Tool

A practical, efficient, high-performance C++ command-line application designed to automate log analysis, detect errors and log levels, normalize and group dynamic error signatures, search log entries by keyword, and generate summarized diagnostic reports.

---

## 1. Problem Statement

In production systems, applications generate massive amounts of log data (`.log` or `.txt` files). Analyzing these files manually to identify root causes of failures, calculate severity frequencies, or spot recurring issues is tedious and error-prone. 

Furthermore, naive log parsing often fails to group identical errors because dynamic parameters (like `user_id: 2091` vs `user_id: 3105` or transaction IDs like `tx_99218`) make identical error signatures appear distinct.

---

## 2. Solution

The **C++ Log Analyzer & Error Monitoring Tool** provides an automated command-line solution built using modern C++ (C++17). It processes log files line-by-line, ignores blank lines, categorizes severity levels (`INFO`, `WARNING`, `ERROR`, `DEBUG`, `UNKNOWN`), normalizes dynamic parameters (such as `user_id`, transaction tokens, and IDs) to group similar errors, and outputs clean diagnostic metrics both to the console and to `reports/report.txt`.

---

## 3. Features & Improvements

* **Robust File I/O Engine**: Processes `.log` and `.txt` files line-by-line with `std::ifstream`, ignoring empty and whitespace-only lines without crashing.
* **Error Signature Grouping (Normalization)**: Uses `std::regex_replace` to mask dynamic values like `user_id: <ID>` and `tx_<ID>`. This prevents error fragmenting and aggregates similar errors under a single signature.
* **Case-Insensitive Keyword Search**: Allows searching for keywords (e.g. `database`, `timeout`, `5`) regardless of uppercase/lowercase formatting.
* **Clean Top Errors Dashboard**: Formatted tabular view displaying total errors logged, unique error patterns count, and ranked frequencies.
* **Detailed Report Export (`reports/report.txt`)**: Exports comprehensive metrics including percentage breakdowns per log level, system health status, unique error pattern counts, and ranked top errors.
* **Robust Crash-Proof Menu**: Uses `std::getline` with whitespace trimming and type conversion (`std::stoi`) so invalid string inputs or accidental empty Enter presses never crash or hang the program.

---

## 4. Tech Stack

* **Language Standard**: C++ (C++17)
* **Standard Template Library (STL)**:
  * `std::vector` (Contiguous storage of entries and sorted pairs)
  * `std::unordered_map` (Hash table for $\mathcal{O}(1)$ level & error frequency aggregation)
  * `std::string` & string algorithms
  * `std::regex` & `std::regex_replace` (Pattern normalization)
  * `std::ifstream` / `std::ofstream` (File stream processing)
  * `std::sort`, `std::transform` (STL algorithms)
  * Exception handling (`std::runtime_error`, `try-catch`)
* **Build Systems**: standard `g++` compiler (MinGW/GCC) or CMake 3.10+

---

## 5. Project Architecture

```
C++project/
├── include/
│   ├── LogEntry.h         # LogEntry class & LogLevel enum definitions
│   ├── LogAnalyzer.h      # Core file reading, parsing, error normalization & search
│   └── ReportGenerator.h  # Presentation layer & report exporter
│
├── src/
│   ├── LogEntry.cpp       # Line parser and helper method implementations
│   ├── LogAnalyzer.cpp    # File I/O, hash map aggregation, regex normalization & sort
│   ├── ReportGenerator.cpp# File export & formatted console UI implementation
│   └── main.cpp           # Robust interactive CLI menu loop
│
├── logs/
│   └── sample.log         # Sample log file with realistic errors and blank lines
│
├── reports/
│   ├── .gitkeep           # Directory placeholder
│   └── report.txt         # Exported analysis report file
│
├── CMakeLists.txt         # CMake build configuration
├── .gitignore             # Ignore build binaries and report outputs
└── README.md              # Project documentation & interview guide
```

---

## 6. How It Works

```
                     ┌───────────────────────────┐
                     │      sample.log file      │
                     └─────────────┬─────────────┘
                                   │ std::ifstream (line by line, skip blanks)
                                   ▼
                     ┌───────────────────────────┐
                     │     LogEntry::parseLine   │
                     └─────────────┬─────────────┘
                                   │
        ┌──────────────────────────┼──────────────────────────┐
        ▼                          ▼                          ▼
 ┌───────────────┐        ┌──────────────────┐       ┌─────────────────┐
 │ LogLevel Enum │        │ std::vector      │       │ Regex Normalizer│
 │  Categorizer  │        │ <LogEntry>       │       │ user_id -> <ID> │
 └───────┬───────┘        └────────┬─────────┘       └────────┬────────┘
         │                         │                          │
         ▼                         ▼                          ▼
 ┌───────────────┐                 │                 ┌─────────────────┐
 │ Level Counts  │                 │                 │ std::unordered  │
 │ (Percentages) │                 │                 │ _map<Err,Count> │
 └───────┬───────┘                 │                 └────────┬────────┘
         │                         │                          │
         └─────────────────────────┼──────────────────────────┘
                                   │
                                   ▼
                     ┌───────────────────────────┐
                     │   ReportGenerator Class   │
                     └─────────────┬─────────────┘
                                   │
                 ┌─────────────────┴─────────────────┐
                 ▼                                   ▼
        ┌───────────────────┐             ┌─────────────────────┐
        │  Terminal Report  │             │  reports/report.txt │
        └───────────────────┘             └─────────────────────┘
```

---

## 7. Sample Output

### Terminal Summary Output
```
========================================
          LOG ANALYZER SUMMARY          
========================================
 File Path    : logs/sample.log
 Total Lines  : 27
----------------------------------------
 INFO         :      8  (29.6%)
 DEBUG        :      4  (14.8%)
 WARNING      :      3  (11.1%)
 ERROR        :     12  (44.4%)
----------------------------------------
 Unique Errors: 4
========================================

================================================================================
                         TOP ERRORS (GROUPED PATTERNS)                          
================================================================================
 Total Errors Logged    : 12
 Unique Error Signatures: 4
--------------------------------------------------------------------------------
 Rank   Count   Error Signature (Normalized Pattern)
--------------------------------------------------------------------------------
 [ 1]      4x   Database connection failed - connection timed out (host: db.internal.local)
 [ 2]      3x   Authentication failed - invalid JWT signature for user_id: <ID>
 [ 3]      3x   Request timeout while calling payment gateway API
 [ 4]      2x   Payment gateway transaction failed for tx_<ID>
================================================================================
```

---

## 8. How to Build & Run

### Quick Command Line (g++)
```powershell
# Compile
g++ -std=c++17 -Wall -Wextra -Iinclude src/main.cpp src/LogEntry.cpp src/LogAnalyzer.cpp src/ReportGenerator.cpp -o LogAnalyzer.exe

# Run
.\LogAnalyzer.exe
```

---

## 9. C++ Concepts Demonstrated

| Concept | Implementation in Project |
|---|---|
| **Classes & Encapsulation** | Modular classes (`LogEntry`, `LogAnalyzer`, `ReportGenerator`) protecting internal state. |
| **Enum Classes (`enum class`)** | Scoped, strongly-typed `LogLevel` enum preventing implicit conversions. |
| **Regular Expressions (`std::regex`)** | `std::regex_replace` masks variable tokens (`user_id`, `tx_id`) to group identical error root causes. |
| **Hash Maps (`std::unordered_map`)** | Average $\mathcal{O}(1)$ time complexity for tallying severity levels and error frequencies. |
| **STL Algorithms & Lambdas** | `std::sort` with custom lambda for descending error rankings; `std::transform` with `std::tolower` for case-insensitive search. |
| **File I/O Streams (`std::ifstream`, `std::ofstream`)** | Buffered line-by-line reading with blank line filtration and formatted report output generation. |
| **Const-Correctness & References** | Pass-by-const-reference (`const std::string&`, `const LogAnalyzer&`) preventing costly deep copies. |
| **Robust Stream Parsing** | Reading menu inputs with `std::getline` and `std::stoi` to eliminate stream failure crashes. |
| **Exception Handling (`try-catch`)** | Graceful error recovery using `std::runtime_error` when files are missing or unreadable. |

---

## 10. Complexity Analysis

* **File Reading & Parsing**: $\mathcal{O}(N)$ where $N$ is the number of non-blank lines.
* **Error Grouping & Normalization**: $\mathcal{O}(N \times K)$ where $K$ is the regex pattern length on error messages.
* **Level & Error Counting**: Average $\mathcal{O}(1)$ hash map operations per line.
* **Top Error Sorting**: $\mathcal{O}(U \log U)$ where $U$ is the number of unique error signatures ($U \le N$).
* **Keyword Search**: $\mathcal{O}(N \times L)$ where $L$ is average line length, using `std::string::find`.
