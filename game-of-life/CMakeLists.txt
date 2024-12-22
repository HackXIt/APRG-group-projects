cmake_minimum_required(VERSION 3.10)
project(game_of_life)

#set(GUI ON CACHE BOOL "Build with GUI support")
# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
# Set optimization flags for the Release build type
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

include(FetchContent)

FetchContent_Declare(
        cxxopts
        GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
        GIT_TAG v3.2.1
)
FetchContent_MakeAvailable(cxxopts)

# Fetch the source code of SFML (if GUI defined)
if(GUI)
    include(FetchContent)
    FetchContent_Declare(SFML
            GIT_REPOSITORY https://github.com/SFML/SFML.git
            GIT_TAG 2.6.x
            GIT_SHALLOW ON
            EXCLUDE_FROM_ALL
            SYSTEM)
    FetchContent_MakeAvailable(SFML)
endif()

# Add include directory where the header files are located
include_directories(${CMAKE_CURRENT_LIST_DIR}/include ${cxxopts_SOURCE_DIR}/include)

# Create a library for the maze-solving logic
add_library(${PROJECT_NAME}_lib src/${PROJECT_NAME}.cpp)

# Add the main program, linking it to the game_of_life_lib
if(GUI)
    add_compile_definitions(GUI)
    add_executable(${PROJECT_NAME} src/main.cpp src/Timing.cpp src/gui.cpp)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib sfml-graphics -static-libgcc -static-libstdc++)
else()
    add_executable(${PROJECT_NAME} src/main.cpp src/Timing.cpp)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib -static-libgcc -static-libstdc++)
endif()
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_lib)

# Google Test setup
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/release-1.12.1.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add the Google Test executable for testing
enable_testing()
file(GLOB TEST_FILES test/test_*.cpp)
add_executable(${PROJECT_NAME}_test ${TEST_FILES} src/Timing.cpp)
target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME}_lib gtest_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)

# Define input and expected directories
file(GLOB_RECURSE INPUT_FILES "${CMAKE_CURRENT_LIST_DIR}/input/*")
file(GLOB_RECURSE EXPECTED_FILES "${CMAKE_CURRENT_LIST_DIR}/expected/*")

# Copy individual files from input/ & expected/ to the build directory
foreach(FILE ${INPUT_FILES})
    get_filename_component(FILENAME ${FILE} NAME)
    configure_file(${FILE} ${CMAKE_CURRENT_BINARY_DIR}/input/${FILENAME} COPYONLY)
endforeach()

foreach(FILE ${EXPECTED_FILES})
    get_filename_component(FILENAME ${FILE} NAME)
    configure_file(${FILE} ${CMAKE_CURRENT_BINARY_DIR}/expected/${FILENAME} COPYONLY)
endforeach()

add_dependencies(${PROJECT_NAME}_test ${PROJECT_NAME}_lib)