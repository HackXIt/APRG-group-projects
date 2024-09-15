# Introduction

This guide provides step-by-step instructions to set up the development environment for the APRG-collision-detection project using SDL2.
The setup process includes installing necessary dependencies, configuring CMake, and ensuring the environment is ready for development.

The guide also assumes the **windows operating system**, since that is what all team members are using.

SDL2 (Simple DirectMedia Layer 2) is a cross-platform development library used to provide low-level access to audio, keyboard, mouse, joystick, and graphics hardware.

# Setup dependencies

## 1. Install SDL2

- Download the development libraries from the SDL2 Downloads page.
- Extract the downloaded ZIP file.
- Copy the include folder and lib folder (appropriate for your architecture, e.g., x64) into a location where you can easily reference them.

## 2. Setup SDL2 in project

1. Configure CMAKE project

The project setup will be performed with the provided CMakeLists file.

```shell
# Execute in root project directory (APRG-group-projects)
cmake .
```

The following lines are responsible for locating SDL2:

```cmake
# Find SDL2 package
find_package(SDL2 REQUIRED)

# Include SDL2 headers
include_directories(${SDL2_INCLUDE_DIRS})

# Link SDL2 libraries
target_link_libraries(collision-detection ${SDL2_LIBRARIES})
```

2. Build the example project using SDL2

```shell
# Execute in root project directory (APRG-group-projects)
cmake --build . --target sdl2-example
```

