cmake_minimum_required(VERSION 3.15)
project(ChessEngineProject VERSION 1.0 LANGUAGES CXX)

# Force C++17 or higher for modern features
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------------------------------------------------------
# 1. DEPENDENCY: Find SFML (For the Graphical User Interface)
# -----------------------------------------------------------------------------
# On Windows/MSYS2, make sure your CMAKE_PREFIX_PATH includes your compiler paths if not found
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# -----------------------------------------------------------------------------
# 2. DEPENDENCY: Find Google Test (GTest)
# -----------------------------------------------------------------------------
find_package(GTest REQUIRED)
include(GoogleTest)

# -----------------------------------------------------------------------------
# 3. BUILD TARGET: Main Chess GUI Application
# -----------------------------------------------------------------------------
add_executable(chess_gui InteractiveBoard/main_gui.cpp)

# Link SFML libraries to the main game binary
target_link_libraries(chess_gui PRIVATE sfml-graphics sfml-window sfml-system)

# -----------------------------------------------------------------------------
# 4. BUILD TARGET: Unit Testing Suite
# -----------------------------------------------------------------------------
enable_testing()

add_executable(engine_tests Test_GameEngine.cpp)

# Link GTest, threading libraries, and standard test libraries
target_link_libraries(engine_tests PRIVATE GTest::gtest GTest::gtest_main pthread)

# Automatically register the gtest binaries with CMake's test runner
gtest_discover_tests(engine_tests)