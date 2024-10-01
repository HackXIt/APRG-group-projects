# Convex Hull Algorithms Visualization and Performance Test

![](showcase/Convex-Hull_Algorithm-Visualization.mp4)

## Introduction

This project implements and visualizes various convex hull algorithms, specifically:

- **QuickHull**
- **Jarvis March**
- **Divide & Conquer** _(not implemented yet)_

The program provides both a graphical user interface (GUI) for visualization and a command-line interface (CLI) for performance testing.

## Features

- **Algorithm Visualization**: Step-by-step visualization of convex hull algorithms for educational purposes.
- **CLI Mode**: Run algorithms on large datasets for performance measurement.
- **Test Data Generation**: Python scripts to generate test datasets with various point distributions.
- **Automated Testing**: Scripts to execute algorithms on test datasets and log the results.

## Installation

To build and run the program, you need:

- **C++ Compiler**: GCC with MinGW-w64 for Windows. Use [this specific version](https://github.com/niXman/mingw-builds-binaries/releases/download/13.1.0-rt_v11-rev1/x86_64-13.1.0-release-win32-seh-msvcrt-rt_v11-rev1.7z) to avoid compatibility issues with SFML.
- **CMake**: For building the project.
- **SFML Library**: For GUI visualization.

### Building the Project

1. **Clone the Repository**:

   ```bash
   git clone https://github.com/HackXIt/APRG-group-projects.git
   ```

2. **Run CMake Configuration:**

    ```bash
   # Optional: Create a build directory
    mkdir build && cd build
    cmake ..
    ```

3. **Navigate to the Sub-Project inside the build directory:**

    ```bash
    cd convex-hull
    ```

4. Build the Project:

    ```bash
    cmake --build .
    ```
   
The executable will be located in the bin directory.

## Usage

The program can be run in two modes:

 - GUI Mode: For visualization _(limited to a maximum of **50 data points**)_.
 - CLI Mode: For performance testing on larger datasets.

### Command-Line Interface (CLI)

The CLI allows you to execute the convex hull algorithms on specified datasets.

#### Syntax

```bash
convex-hull.exe [OPTIONS] data_file
```

#### Options
 * `-h`, `--help`: Print help message.
 * `-g`, `--gui`: Run with visualization using pre-loaded data (limited to less than 50 points).
 * `-d`, `--data_file FILEPATH`: Path to a file containing points to load.
 * `-a`, `--algorithm MODE`: Algorithm to use. 
   * `0`: QuickHull
   * `1`: Jarvis March
   * `2`: Divide & Conquer.

#### Data File Format
 * First Line: Contains the number of points (integer). 
 * Subsequent Lines: Each line contains the X and Y coordinates of a point, space-separated (floating-point values).

#### Example

```bash
convex-hull.exe -a 0 -d "path/to/data_file.txt"
```

### Graphical User Interface (GUI)

The GUI provides a step-by-step visualization of the convex hull algorithms.

#### Running in GUI Mode

```bash
convex-hull.exe -g -a 0 -d "path/to/data_file.txt"
```

**Note:** The GUI mode is limited to datasets with fewer than 50 points.

##### Controls
 * `SPACE`: Step through the algorithm with visual explanation.
 * `R`: Reset the visualization.
 * `ENTER`: Run the algorithm to completion. *(1)
 * `Click in UI`: Place points manually. *(1)

_*(1) Only possible if calculation has not started yet. If calculation already started, a reset is required._

### Test Data Generator

The project includes Python scripts to generate test datasets with various point distributions.

#### Generating Test Data

The test data generator creates datasets with the following configurations:

 - Random Distribution of Points
 - Points Forming a Straight Line
 - Points Forming a Circle
 - Random Distribution Inside a Square
 - Large Dataset with 100 Million Points

**Script:** [`generate_test_data.py`](test/generate_test_data.py)

The `generate_test_data.py` script contains functions to generate different types of test data. You can find the script in the repository under the scripts directory.

##### Example Usage

```bash
python generate_test_data.py
```

Note: Modify the script as needed to generate different datasets.

##### Data Files Naming Convention

Test data files are named using the pattern:

```
TC_<TestName>_<DataPoints>.txt
```

For example:

 * `TC_RandomPoints_10.txt`
 * `TC_CirclePoints_1000.txt`

#### Test Script

Automated testing is facilitated using a PowerShell script.

**PowerShell Script:** [`run_tests.ps1`](test/run_tests.ps1)

The PowerShell script executes the compiled binary with the generated test data files and logs the results.

##### Usage

```powershell
.\run_tests.ps1 -BinaryPath "C:\path\to\convex-hull.exe" -LogFile "C:\path\to\test.log" # Optional: Log file path
```

##### Script Overview

 * Parameters:
   * -BinaryPath: Full path to the convex-hull.exe executable.
 * Functionality:
   * Iterates over all test data files matching TC_*.txt.
   * Executes the convex hull algorithms (0: QuickHull, 1: Jarvis March, 2: Divide & Conquer) on each test file.
   * Logs the output and errors to test.log.

##### Example Execution

```powershell

# Navigate to the directory containing the script and test data files
cd C:\path\to\scripts

# Run the test script
.\run_tests.ps1 -BinaryPath "C:\path\to\convex-hull.exe"
```

##### Examples

 - Running QuickHull Algorithm on a Dataset
    ```bash
    convex-hull.exe -a 0 -d "TC_RandomPoints_1000.txt"
    ```

 - Running Jarvis March Algorithm with Visualization

    ```bash
    convex-hull.exe -g -a 1 -d "TC_CirclePoints_10.txt"
    ```

# License

This project is licensed under the MIT License. See the [LICENSE](../LICENSE) file for details.
