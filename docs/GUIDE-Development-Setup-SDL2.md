# Introduction

This guide provides step-by-step instructions to set up the development environment for the APRG-collision-detection project using SDL2.
The setup process includes installing necessary dependencies, configuring CMake, and ensuring the environment is ready for development.

The guide also assumes the **windows operating system**, since that is what all team members are using.

SDL2 (Simple DirectMedia Layer 2) is a cross-platform development library used to provide low-level access to audio, keyboard, mouse, joystick, and graphics hardware.

# Setup dependencies

For SDL2, I haven't found any specific or necessary MinGW version.

It should work with the same MinGW version you should have downloaded with the SFML example or also with the bundled version by CLion.

## 1. Setup SDL2 in project

1. Configure CMAKE project

The project setup will be performed with the provided CMakeLists file.

```shell
# Execute in root project directory (APRG-group-projects)
cmake .
```

If the above does not work, since you might not have the `cmake` binary on your path, you can use the CLion IDE to configure.

- Open the `sfml-example/CMakeLists.txt` file in CLion.
- Click the `Reload CMake Project` button in the top right corner.
- CLion will automatically configure the project with the provided CMakeLists file.

Be aware that the first time configuring with CMake will take a while, since it will download the SDL2 source code.

The following lines are responsible for locating and linking SDL2:

```cmake
# Fetch the source code of SFML
include(FetchContent)
FetchContent_Declare(SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.30.7
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SDL2)

# Define the executable target
add_executable(sdl2-example src/main.cpp)
target_include_directories(sdl2-example PRIVATE include)

# Link SDL2 libraries properly
target_link_libraries(sdl2-example PRIVATE SDL2::SDL2 SDL2::SDL2main)

# Specify that we're using C++17
target_compile_features(sdl2-example PRIVATE cxx_std_17)

# Correctly locate SDL2.dll and set up the copy command for Windows
if (WIN32)
    add_custom_command(
            TARGET sdl2-example POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL2::SDL2> $<TARGET_FILE_DIR:sdl2-example>
    )
endif()
```

2. Build the example project using SDL2

```shell
# Execute in root project directory (APRG-group-projects)
cmake --build . --target sdl2-example
```

Again, if the above does not work, you can use the CLion IDE to build the project.

- Select the `sdl2-example` target in the `Run/Debug Configurations` dropdown.
- Click the `Build` button in the top right corner of the CLion IDE.
- CLion will automatically build the project with the provided CMakeLists file.

If the build is successful, you should see the following output:

```shell
...
[475/476] Linking C static library _deps\sdl2-build\libSDL2d.a
[476/476] Linking CXX executable sdl2-example\bin\sdl2-example.exe

Build finished
```

3. Run the example project

```shell
# Execute in root project directory (APRG-group-projects)
./cmake-build-debug/sdl2-example/sdl2-example.exe
```

Or alternatively you can run the executable from the CLion IDE.

- Select the `sdl2-example` target in the `Run/Debug Configurations` dropdown.
- Click the `Run` button in the top right corner of the CLion IDE.
- CLion will automatically run the project with the provided CMakeLists file.

The program should simply open a window using SDL2.

If it crashes or does not open, or something went wrong, you should contact @HackXIt

