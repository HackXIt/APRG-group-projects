# Define the project for collision-detection
project(sfml-example)

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

# Add the executable and link SFML
add_executable(sfml-example src/main.cpp)
target_link_libraries(sfml-example PRIVATE sfml-graphics)
target_compile_features(sfml-example PRIVATE cxx_std_17)

# On windows we need to copy the DLL to the output path
if(WIN32)
    add_custom_command(
            TARGET sfml-example
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:sfml-example>
            VERBATIM)
endif()