# Introduction

This guide provides step-by-step instructions to set up the development environment for the APRG-collision-detection project using SFML (Simple and Fast Multimedia Library).
The setup process includes installing necessary dependencies, configuring CMake, and ensuring the environment is ready for development.

The guide also assumes the **windows operating system**, since that is what all team members are using.

SFML is a versatile and easy-to-use library used for developing multimedia applications, including games and graphical programs, by providing access to graphics, sound, and input/output devices.

# Setup dependencies

## 1. Install SFML

- Download the latest version of the SFML development libraries from the SFML Downloads page.
- Extract the downloaded ZIP file.
- Copy the include folder, lib folder, and the bin folder (containing .dll files) to a location where you can reference them in your project.

## 2. Setup SFML in project

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