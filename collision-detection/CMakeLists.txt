# Define the project for collision-detection
project(collision-detection)

# Add include directories
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

# Add the executable target for collision-detection
add_executable(collision-detection
        ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
        ../sfml-example/src/main.cpp
        ../sdl2-example/src/main.cpp # Replace with your actual source files
)

# Link libraries if needed
# target_link_libraries(collision-detection <library-name>)
