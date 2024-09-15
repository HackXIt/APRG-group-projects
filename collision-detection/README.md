# APRG-collision-detection

# Projekt-Aufteilung

- [x] Projekt mit CMake konfigurieren (... libSDL2 oder SFML hinzufügen, etc...)
=> sollte bis morgen 16.09 ready sein (Leeres Fenster mit Beispiel-Library-Code)
- [ ] Klassen zur Anwendung der Methode einrichten (Plane/Triangle/Sphere/AABB)
=> In library doku zu recherchieren (jeder): Bis Freitag 20.09 
- [ ] Algorithmus zur Kollisionserkennung implementieren (Bis Samstag 21.09) 
  - [ ] Interval Halfing Method
  - [ ] Moving sphere to Plane/Triangle/Sphere/AABB
  - [ ] Window und Renderer für Visualisierung der Collision (DebugLines für Detection)
- [ ] Live-Demo => Schleife der 4 Kollisionen (Sphere to Plane/Triangle/Sphere/AABB) => Bis Sonntag 22.09
- [ ] Dokumentation => Bis Dienstag vor Abgabe 24.09
  - [ ] Testen der Kollisionserkennung

# Dependencies

# References

- [SFML](https://www.sfml-dev.org/)
- [SDL2 Wiki](https://wiki.libsdl.org/SDL2/FrontPage)

# How-to-build

These build instructions assume a basic knowledge of how to work with CMake, CLion and the command line.

There is an additional GUIDE with detailed step-by-step instructions in the `docs` folder, named:
- [`GUIDE-Development-Setup-SFML.md`](../docs/GUIDE-Development-Setup-SFML.md) for setting up the project with SFML
- [`GUIDE-Development-Setup-SDL2.md`](../docs/GUIDE-Development-Setup-SDL2.md) for setting up the project with SDL2

The following instructions assume that you have already completed the steps in the setup guide.

_Simple approach - **you will require CLion for this**_
1. Checkout the project in CLion
2. Open the project and let CLion configure cmake
3. Build the project using the provided CMake configuration

_Command line approach - **you will require the necessary tools installed and on PATH for this**_
1. Clone the repository
2. Run the following commands in the project root directory:
```shell
cmake --build . --target collision-detection
# Optional for faster build
cmake --build . --target collision-detection -- -j4
```
3. Run the executable in the `build` directory
```shell
./cmake-build-debug/collision-detection/collision-detection.exe
```

# How-to-run

There are pre-built binaries available in the repository.
Download a release and run the executable.