//
// Created by RINI on 21/12/2024.
//

#include "game_of_life.h"
#include <iostream>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>\n";
    return 1;
  }

  try {
    GameOfLife game = GameOfLife::fromFile(argv[1]);
    for (int i = 0; i < 100; ++i) { // Simulate 100 generations
      game.update();
    }
    game.toFile("output.txt");
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}