# Define the project for collision-detection
project(sdl2-example)

# Add include directories
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

# Add the executable target for collision-detection
add_executable(sdl2-example
        ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp # Replace with your actual source files
)

# Link libraries if needed
# target_link_libraries(collision-detection <library-name>)
