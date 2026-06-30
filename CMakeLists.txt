cmake_minimum_required(VERSION 3.15)
project(ChessTwoPlayer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(chess_gui
    main.cpp
    GameEngine/Sidepanel/CaptureTracker.cpp
)

target_link_libraries(chess_gui PRIVATE sfml-graphics sfml-window sfml-system)

option(BUILD_ENGINE_TESTS "Build chess engine unit tests" ON)

if(BUILD_ENGINE_TESTS)
    find_package(GTest QUIET)

    if(GTest_FOUND)
        include(GoogleTest)
        enable_testing()

        add_executable(engine_tests
            Test/Test_GameEngine.cpp
            GameEngine/Sidepanel/CaptureTracker.cpp
        )

        target_link_libraries(engine_tests PRIVATE GTest::gtest GTest::gtest_main)
        gtest_discover_tests(engine_tests)
    else()
        message(STATUS "GTest not found; skipping engine_tests target")
    endif()
endif()
