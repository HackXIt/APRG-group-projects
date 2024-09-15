# Introduction

This guide provides step-by-step instructions to set up the development environment for the APRG-collision-detection project using SFML (Simple and Fast Multimedia Library).
The setup process includes installing necessary dependencies, configuring CMake, and ensuring the environment is ready for development.

The guide also assumes the **windows operating system**, since that is what all team members are using.

SFML is a versatile and easy-to-use library used for developing multimedia applications, including games and graphical programs, by providing access to graphics, sound, and input/output devices.

# Setup dependencies

**The guide assumes SFML version 2.6.1 and will therefor use MinGW (SEH) 13.1.0.**

## 1. Install MinGW

- Download [**THIS SPECIFIC VERSION**](https://github.com/niXman/mingw-builds-binaries/releases/download/13.1.0-rt_v11-rev1/x86_64-13.1.0-release-win32-seh-msvcrt-rt_v11-rev1.7z) of minGW for CLion
  - If the above link is broken, go to: https://github.com/niXman/mingw-builds-binaries/releases/tag/13.1.0-rt_v11-rev1
  - Then download the version `x86_64-13.1.0-release-win32-seh-msvcrt-rt_v11-rev1`.
- Extract the downloaded 7z file to a location where you can reference it in your project.
  - For example, `C:\Tools\mingw64`
  - The extracted folder should contain a `bin` folder with the MinGW executables.
- Open CLion and navigate to `File > Settings > Build, Execution, Deployment > Toolchains`.
- Click the `+` icon and select `MinGW` from the dropdown menu.
- Set the `Environment` field to the path of the extracted MinGW folder.
  - For example, `C:\Tools\mingw64`
- Move the new MinGW toolchain to the top of the list by clicking the up arrow.
  - Make sure it is highlighted in bold for it to be used.

## 2. Setup SFML in project

1. Configure CMAKE project

The project setup will be performed with the provided CMakeLists file.

```shell
# Execute in root project directory (APRG-group-projects)
cmake .
```

The following lines are responsible for locating SFML:

```cmake
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
```

2. Build the example project using SFML

```shell
# Execute in root project directory (APRG-group-projects)
cmake --build . --target sfml-example
```

If the build is successful, you should see the following output:

```shell
...
[100%] Built target sfml-example
```

3. Run the example project

```shell
# Execute in root project directory (APRG-group-projects)
./cmake-build-debug/sfml-example/sfml-example.exe
```

The program should open a window with a simple SFML example.

If it crashes or does not open, or something went wrong, you should contact @HackXIt

# (OPTIONAL) Manual installation of pre-built SFML

**This step is optional and NOT REQUIRED when using CMake for fetching the SFML sources.**

I have added it only, so that there is a backup plan for linking pre-built SFML libraries.

- Download the SFML 2.6.1 version from the official website: https://www.sfml-dev.org/download/sfml/2.6.1/
  - Download the `GCC 13.1.0 MinGW (SEH) - 64-bit` version.
- Extract the downloaded zip file to a location where you can reference it in your project.
  - For example, `C:\Tools\SFML-2.6.1`
  - The extracted folder should contain an `include` folder, a `lib` folder, and a `bin` folder.
- Add the SFML bin directory to the system PATH.
  - Open the system environment variables settings.
  - Add a new system variable with the name `SFML_ROOT` and the value of the SFML directory.
    - For example, `C:\Tools\SFML-2.6.1`
  - Edit the `Path` system variable and add the SFML bin directory.
    - For example, `C:\Tools\SFML-2.6.1\bin` 

To use the pre-built SFML libraries, you can modify the CMakeLists.txt file to link the libraries directly:

```cmake
# Add the executable and link SFML
add_executable(sfml-example src/main.cpp)
target_link_libraries(sfml-example PRIVATE
        ${SFML_ROOT}/lib/libsfml-graphics.a
        ${SFML_ROOT}/lib/libsfml-window.a
        ${SFML_ROOT}/lib/libsfml-system.a
        ${SFML_ROOT}/lib/libsfml-audio.a
        ${SFML_ROOT}/lib/libsfml-network.a
        ${SFML_ROOT}/lib/libsfml-main.a
        ${SFML_ROOT}/lib/libsfml-graphics-s.a
        ${SFML_ROOT}/lib/libsfml-window-s.a
        ${SFML_ROOT}/lib/libsfml-system-s.a
        ${SFML_ROOT}/lib/libsfml-audio-s.a
        ${SFML_ROOT}/lib/libsfml-network-s.a
        ${SFML_ROOT}/lib/libsfml-main-s.a
        )
target_compile_features(sfml-example PRIVATE cxx_std_17)
```