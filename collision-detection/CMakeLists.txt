# Define the project for collision-detection
project(collision-detection)

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

# Add the executable and link SFML
add_executable(${PROJECT_NAME} ${SOURCES})

# Copy the font file to the build directory
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../resources/FiraCodeNerdFontMono-Retina.ttf
        ${CMAKE_CURRENT_BINARY_DIR}/resources/FiraCodeNerdFontMono-Retina.ttf
        COPYONLY)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_CURRENT_LIST_DIR}/../extern/epsilon/include)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# On windows we need to copy the DLL to the output path
if(WIN32)
    add_custom_command(
            TARGET ${PROJECT_NAME}
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:collision-detection>
            VERBATIM)
endif()
