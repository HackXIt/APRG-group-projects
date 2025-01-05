# Conway's Game of Life

This project implements Conway's Game of Life in C++. The program supports both a command-line interface and an optional graphical user interface (GUI) for simulation visualization.

## Project Structure

The project is organized into the following directories:

- **`expected/`**\
  Contains the expected output files for testing. Each file corresponds to the output after running a specific number of generations on a test input.

- **`include/`**\
  Contains the header files used in the project.

- **`input/`**\
  Contains test input files with predefined board configurations. The file names indicate the number of generations to simulate.

- **`src/`**\
  Contains the source files for the application logic and optional GUI components.

- **`test/`**\
  Contains Google Test test suites to validate the application logic against predefined inputs and expected outputs.

## Run

To simulate Conway's Game of Life, execute the program using the following options:

```shell
.\game_of_life.exe -h
 - Simulate Conway's Game of Life
Usage:
  C:\Users\rini\DATA\git-stash\APRG-group-projects\cmake-build-release\game-of-life\game_of_life.exe [OPTION...]

  -l, --load arg         Filename to read input board
  -s, --save arg         Filename to save output board (default:
                         output.gol)
  -g, --generations arg  Number of generations to simulate (default: 100)
  -m, --measure          Print time measurements
      --gui arg          Enable graphical user interface (arg==cell size)
                         (default: 25)
  -h, --help             Print usage
```

### Command-Line Options

- **`-l, --load [arg]`**
  Specify the input filename to load the initial board configuration.

- **`-s, --save [arg]`**
  Specify the output filename to save the board after simulation (default: `output.gol`).

- **`-g, --generations [arg]`**
  Set the number of generations to simulate (default: `100`).

- **`-m, --measure`**
  Print the time measurements for the simulation process.

- **`--gui [arg]`**
  Enable the graphical user interface. Specify the cell size for the GUI (default: `25`).

- **`-h, --help`**
  Print usage instructions.

### Example Usage

#### Command-Line Simulation

```shell
game_of_life.exe -l input/test1.txt -g 50 -s output.gol
```

#### GUI Simulation

```shell
game_of_life.exe --gui 30 -l input/<testfile>.gol -g 100
```

## Build

### Prerequisites

Ensure you have the following installed:

- `cmake` (minimum version 3.10)
- A C++17-compatible compiler (e.g., GCC, Clang)

### Building the Application

#### Without GUI Support

```shell
mkdir build && cd build
cmake -DGUI=OFF ..
cmake --build .
```

#### With GUI Support

```shell
mkdir build && cd build
cmake -DGUI=ON ..
cmake --build .
```

### Build Optimization

For optimized builds (Release mode):

```shell
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

## Test

This application includes a robust test suite implemented using Google Test.

### Running Tests

To run all tests:

```shell
./game_of_life_test
```

The test suite validates the application logic by comparing the output of simulated generations against expected results stored in the `expected/` directory.

## GUI (Optional)

The GUI provides a visual representation of the simulation. It is implemented using SFML (Simple and Fast Multimedia Library). You can enable GUI support during the build process by setting the `GUI` flag to `ON`.

### Running the GUI

To launch the GUI:

```shell
game_of_life.exe --gui 25 -l input/input.gol
```

The GUI displays the board with each cell's state updated in real-time as the simulation progresses.

**The input file is required to determine the initial size and state of the grid.**

### GUI Interactions

- Press `SPACEBAR` to iterate one generation forward. You can also hold spacebar for multiple generations.
- Use `LEFT_MOUSE_CLICK` on a given cell to either activate or deactivate it. *(depends on current cell state)*