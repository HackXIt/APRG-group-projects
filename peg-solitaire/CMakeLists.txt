set (CMAKE_CXX_STANDARD 20)
cmake_minimum_required(VERSION 3.28)
# Define the project for convex-hull
project(peg-solitaire)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Set optimization flags for the Release build type
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

FILE(GLOB SOURCES src/*.cpp)

# Add the executable target for convex-hull
add_executable(${PROJECT_NAME}
        ${SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
        )
target_link_libraries(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)