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

## 2. Install SFML

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

## 3. Setup SFML in project

1. Configure CMAKE project

The project setup will be performed with the provided CMakeLists file.

```shell
# Execute in root project directory (APRG-group-projects)
cmake .
```

The following lines are responsible for locating SFML:

```cmake
# Find the SFML package
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Include SFML headers
include_directories(${SFML_INCLUDE_DIRS})

# Link SFML libraries to your executable
target_link_libraries(collision-detection sfml-graphics sfml-window sfml-system)
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

