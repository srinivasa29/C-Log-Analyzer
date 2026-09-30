cmake_minimum_required(VERSION 3.10)
project(LogAnalyzer VERSION 1.0 LANGUAGES CXX)

# Require modern C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include header directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/LogEntry.cpp
    src/LogAnalyzer.cpp
    src/ReportGenerator.cpp
)

# Target executable
add_executable(LogAnalyzer ${SOURCES})

# Output binary into top build directory
set_target_properties(LogAnalyzer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
