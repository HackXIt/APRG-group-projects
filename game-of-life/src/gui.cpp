//
// Created by RINI on 22/12/2024.
//

#include "gui.h"
#include <SFML/Graphics.hpp>

int runGui(GameOfLife& game, int cellSize) {
    int rows = game.getRows();
    int columns = game.getColumns();

    const int padding = 50; // Padding around the grid
    const int minWindowSize = 400; // Minimum window size

    int gridWidth = columns * cellSize;
    int gridHeight = rows * cellSize;

    int windowWidth = std::max(gridWidth + 2 * padding, minWindowSize);
    int windowHeight = std::max(gridHeight + 2 * padding, minWindowSize);

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Conway's Game of Life");
    window.setFramerateLimit(60);

    sf::RectangleShape cellShape(sf::Vector2f(cellSize - 1, cellSize - 1));
    cellShape.setFillColor(sf::Color::Green);

    sf::VertexArray gridLines(sf::Lines);
    // Calculate centered grid origin
    int offsetX = (windowWidth - gridWidth) / 2;
    int offsetY = (windowHeight - gridHeight) / 2;

    for (int x = 0; x <= gridWidth; x += cellSize) {
        gridLines.append(sf::Vertex(sf::Vector2f(x + offsetX, offsetY), sf::Color(128, 128, 128)));
        gridLines.append(sf::Vertex(sf::Vector2f(x + offsetX, gridHeight + offsetY), sf::Color(128, 128, 128)));
    }
    for (int y = 0; y <= gridHeight; y += cellSize) {
        gridLines.append(sf::Vertex(sf::Vector2f(offsetX, y + offsetY), sf::Color(128, 128, 128)));
        gridLines.append(sf::Vertex(sf::Vector2f(gridWidth + offsetX, y + offsetY), sf::Color(128, 128, 128)));
    }


    bool updated = true;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    game.next();
                    updated = true;
                }
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    int mouseX = event.mouseButton.x;
                    int mouseY = event.mouseButton.y;

                    if (mouseX >= offsetX && mouseX < gridWidth + offsetX &&
                        mouseY >= offsetY && mouseY < gridHeight + offsetY) {
                        int col = (mouseX - offsetX) / cellSize;
                        int row = (mouseY - offsetY) / cellSize;

                        if (col >= 0 && col < columns && row >= 0 && row < rows) {
                            if (game.cellState(row, col) == LIVE_CELL) {
                                game.clearCell(row, col);
                            } else {
                                game.setCell(row, col);
                            }
                        }
                        updated = true;
                    }
                }
            }
        }

        if(updated)
        {
            window.clear(sf::Color::Black);
            window.draw(gridLines);
            const auto grid = game.getGrid();
        	for (unsigned int i = 0; i < grid.size(); ++i) {
            	for (unsigned int j = 0; j < grid[i].size(); ++j) {
                	if (grid[i][j] == LIVE_CELL) {
                    	cellShape.setPosition(j * cellSize + offsetX, i * cellSize + offsetY);
                    	window.draw(cellShape);
	                }
    	        }
	        }
            updated = false;
            window.display();
        }
    }
    delete &game;
    delete &cellShape;
    delete &gridLines;
    delete &window;
    return 0;
}