Peg Solitaire Solver
====================

This program solves the peg-solitaire problem and outputs a list of moves in the specified format.

Files:
- [`main.cpp`](./src/main.cpp): Main file containing the main function.
- [`peg_solitaire.h`](./include/peg-solitaire.h) : Header file containing class and struct definitions for peg solitaire.
- [`peg_solitaire.cpp`](./src/peg-solitaire.cpp) : Source file containing the implementation for peg solitaire.
- [`Makefile`](Makefile) : Build instructions.
- [`CMakeLists.txt`](CMakeLists.txt) : Alternative build instructions using `cmake`.
- [`README`](README.md) : Instructions for building and running. _(You are reading this file)_

Building:
---------

To build the program, use the provided Makefile.

```bash
make
```

Alternatively, you can use `cmake` to build the program.

```bash
cmake -S . -B cmake-build
cmake --build cmake-build
```

Running:
--------

To run the program, use the following command:

- When built with `make` instructions:
```bash
./build/peg-solitaire
```

- When built with `cmake` instructions:
```bash
./cmake-build/peg-solitaire
```

Expected Output:
--------

```
.\peg-solitaire.exe
Initial board state:
[[-,-,o,o,o,-,-],
[-,-,o,o,o,-,-],
[o,o,o,o,o,o,o],
[o,o,o,.,o,o,o],
[o,o,o,o,o,o,o],
[-,-,o,o,o,-,-],
[-,-,o,o,o,-,-]]
[[[2,4],[3,4],[4,4]],[[3,2],[3,3],[3,4]],[[1,3],[2,3],[3,3]],[[1,5],[1,4],[1,3]],[[3,4],[3,3],[3,2]],[[3,1],[3,2],[3,3]],[[3,5],[2,5],[1,5]],[[3,7],[3,6],[3,5]],[[4,3],[3,3],[2,3]],[[1,3],[2,3],[3,3]],[
[4,1],[4,2],[4,3]],[[4,3],[3,3],[2,3]],[[4,5],[3,5],[2,5]],[[1,5],[2,5],[3,5]],[[4,7],[4,6],[4,5]],[[4,5],[3,5],[2,5]],[[6,3],[5,3],[4,3]],[[5,1],[5,2],[5,3]],[[5,3],[4,3],[3,3]],[[2,3],[3,3],[4,3]],[[4
,3],[4,4],[4,5]],[[5,5],[4,5],[3,5]],[[2,5],[3,5],[4,5]],[[5,7],[5,6],[5,5]],[[5,4],[5,5],[5,6]],[[7,5],[6,5],[5,5]],[[4,5],[5,5],[6,5]],[[7,3],[7,4],[7,5]],[[7,5],[6,5],[5,5]],[[5,6],[5,5],[5,4]],[[6,4],[5,4],[4,4]]]
Final board state:
[[-,-,.,.,.,-,-],
[-,-,.,.,.,-,-],
[.,.,.,.,.,.,.],
[.,.,.,o,.,.,.],
[.,.,.,.,.,.,.],
[-,-,.,.,.,-,-],
[-,-,.,.,.,-,-]]
```