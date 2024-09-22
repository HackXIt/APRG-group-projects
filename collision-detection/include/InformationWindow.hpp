//
// Created by RINI on 22/09/2024.
//

#ifndef INFORMATIONWINDOW_H
#define INFORMATIONWINDOW_H
#include <vector>

#include "BoundingVolume.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"


class InformationWindow {
public:
    explicit InformationWindow(sf::Font& font);

    void draw(sf::RenderWindow& window);
    void addTextField(sf::Vector2f position, collision_detection::BoundingVolume* boundingVolume);
    void addTextField(sf::Shape* shape, collision_detection::BoundingVolume* boundingVolume, float offsetX = 0, float offsetY = 100);

private:
    sf::Font& font;
    // Struct to hold text field information from boundingVolumes
    struct TextField
    {
        sf::Text text;
        float offsetX;
        float offsetY;
        sf::Shape* shape; // Shape information
        collision_detection::BoundingVolume* boundingVolume; // Bounding volume information
        sf::FloatRect bounds;
    };
    std::vector<TextField> textFields;  ///< Vector of text fields holding information.

    void updateInformation();
};



#endif //INFORMATIONWINDOW_H
