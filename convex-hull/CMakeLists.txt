# Define the project for convex-hull
project(convex-hull)

# Add include directories
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

# Add the executable target for convex-hull
add_executable(convex-hull
        ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp # Replace with your actual source files
)

# Link libraries if needed
# target_link_libraries(convex-hull <library-name>)
