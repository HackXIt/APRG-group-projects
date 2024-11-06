cmake_minimum_required(VERSION 3.10)
project(maze_solver)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Set optimization flags for the Release build type
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Add include directory where the header files are located
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

# Create a library for the maze-solving logic
add_library(${PROJECT_NAME}_lib src/maze_solver.cpp)

# Add the main program, linking it to the maze_solver_lib
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)

# Copy input and expected output files to the build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/input.txt
        ${CMAKE_CURRENT_BINARY_DIR}/input.txt
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/expected.txt
        ${CMAKE_CURRENT_BINARY_DIR}/expected.txt
)

# Google Test setup
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/release-1.12.1.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add the Google Test executable for testing
enable_testing()
add_executable(${PROJECT_NAME}_test test/test_${PROJECT_NAME}.cpp)
target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME}_lib gtest_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)

# Copy input and expected output files to the build directory for the test executable
add_custom_command(TARGET ${PROJECT_NAME}_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/input.txt
        ${CMAKE_CURRENT_BINARY_DIR}/input.txt
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/expected.txt
        ${CMAKE_CURRENT_BINARY_DIR}/expected.txt
)

# Custom Test Target for Comparing Output
# Platform-specific custom test target
if (WIN32)
    # On Windows, use .exe for the executable and pass file paths as arguments
    add_custom_target(run_${PROJECT_NAME}_test
            COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.exe ${CMAKE_CURRENT_BINARY_DIR}/input.txt ${CMAKE_CURRENT_BINARY_DIR}/actual_output.txt
            COMMAND ${CMAKE_COMMAND} -E compare_files ${CMAKE_CURRENT_BINARY_DIR}/actual_output.txt ${CMAKE_CURRENT_BINARY_DIR}/expected.txt
            COMMENT "Running ${PROJECT_NAME}.exe with input.txt and comparing output with expected.txt"
    )
else()
    # On Unix-like systems, use ./maze_solver
    add_custom_target(run_${PROJECT_NAME}_test
            COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} ${CMAKE_CURRENT_BINARY_DIR}/input.txt ${CMAKE_CURRENT_BINARY_DIR}/actual_output.txt
            COMMAND ${CMAKE_COMMAND} -E compare_files ${CMAKE_CURRENT_BINARY_DIR}/actual_output.txt ${CMAKE_CURRENT_BINARY_DIR}/expected.txt
            COMMENT "Running ${PROJECT_NAME} with input.txt and comparing output with expected.txt"
    )
endif()
add_dependencies(run_${PROJECT_NAME}_test ${PROJECT_NAME})