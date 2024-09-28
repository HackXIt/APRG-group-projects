//
// Created by RINI on 25/09/2024.
//

#ifndef TEXTWINDOW_H
#define TEXTWINDOW_H
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>


class TextWindow {
public:
    explicit TextWindow(sf::Font& font);

    void draw(sf::RenderWindow& window);
    int addTextField(sf::Vector2f position, float offsetX = 0, float offsetY = 0);

    void setText(int textFieldIndex, const std::string &text);

    void updatePosition(int textFieldIndex, sf::Vector2f position, float offsetX = 0, float offsetY = 0);

private:
    sf::Font& font;
    // Struct to hold text field information from boundingVolumes
    struct TextField
    {
        sf::Text field;
        std::string text;
        sf::Vector2f coords;
        float offsetX;
        float offsetY;
    };
    std::vector<TextField> textFields;  ///< Vector of text fields holding information.

    void updateInformation();
};



#endif //TEXTWINDOW_H
