set (CMAKE_CXX_STANDARD 20)
cmake_minimum_required(VERSION 3.28)
# Define the project for convex-hull
project(convex-hull)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Fetch the source code of SFML
include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

FILE(GLOB SOURCES src/*.cpp)

# Add the executable target for convex-hull
add_executable(${PROJECT_NAME}
        ${SOURCES}
)

# Link libraries if needed
# target_link_libraries(convex-hull <library-name>)
# Copy the font file to the build directory
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../resources/FiraCodeNerdFontMono-Retina.ttf
        ${CMAKE_CURRENT_BINARY_DIR}/resources/FiraCodeNerdFontMono-Retina.ttf
        COPYONLY)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/../extern/epsilon/include
        cxxopts
        )
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics cxxopts -static-libgcc -static-libstdc++)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# On windows we need to copy the DLL to the output path
if(WIN32)
    add_custom_command(
            TARGET ${PROJECT_NAME}
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
            VERBATIM)
endif()

# -------------------------------------------------------------------------
# Add a custom target 'run_tests' to execute the compiled binary with all
# variations of test cases using the generator mode.
# -------------------------------------------------------------------------

# Define variables for test cases, algorithms, and data points
set(TEST_CASES 0 1 2 3 4)
set(ALGORITHMS 0 1)
set(DATA_POINTS 10 50 1000 1000000 100000000)

# Get the path to the convex-hull executable
set(CONVEX_HULL_EXECUTABLE $<TARGET_FILE:${PROJECT_NAME}>)

# List to collect all test targets
set(TEST_TARGETS)

# Iterate over all combinations of test cases, algorithms, and data points
foreach(TEST_CASE ${TEST_CASES})
    foreach(ALGORITHM ${ALGORITHMS})
        foreach(NUM_POINTS ${DATA_POINTS})
            # Create a unique name for the test target
            set(TEST_TARGET_NAME test_tc${TEST_CASE}_alg${ALGORITHM}_n${NUM_POINTS})
            # Create the custom target for this test
            add_custom_target(${TEST_TARGET_NAME}
                    COMMAND ${CONVEX_HULL_EXECUTABLE} -a ${ALGORITHM} -t ${TEST_CASE} -n ${NUM_POINTS}
                    DEPENDS ${PROJECT_NAME}
                    COMMENT "Running test case ${TEST_CASE}, algorithm ${ALGORITHM}, data points ${NUM_POINTS}"
            )
            # Add this test target to the list
            list(APPEND TEST_TARGETS ${TEST_TARGET_NAME})
        endforeach()
    endforeach()
endforeach()

# Add the 'run_tests' target that depends on all individual test targets
add_custom_target(run_tests
        DEPENDS ${TEST_TARGETS}
        COMMENT "Running all tests..."
)