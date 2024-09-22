//
// Created by RINI on 22/09/2024.
//

#include "InformationWindow.hpp"

InformationWindow::InformationWindow(sf::Font& font) : font(font) {
}

void InformationWindow::updateInformation() {
    // Update the text positions
    for(auto& textField : textFields) {
        if(textField.shape == nullptr) {
            textField.text.setString(textField.boundingVolume->toString());
        } else
        {
            textField.text.setString("X: "+ std::to_string(textField.shape->getPosition().x)
                + "\nY: " + std::to_string(textField.shape->getPosition().y)
                + "\n" + textField.boundingVolume->toString());
            textField.text.setPosition(textField.shape->getPosition().x + textField.offsetX, textField.shape->getPosition().y + textField.offsetY);
        }
    }
}

void InformationWindow::draw(sf::RenderWindow& window) {

    updateInformation();

    for(auto& textField : textFields) {
        window.draw(textField.text);
    }
}

void InformationWindow::addTextField(sf::Vector2f position, collision_detection::BoundingVolume* boundingVolume){
    // Create a new text field
    TextField textField;
    textField.boundingVolume = boundingVolume;
    textField.shape = nullptr;
    textField.text.setFont(font);
    textField.text.setCharacterSize(12);
    textField.text.setFillColor(sf::Color::White);
    textField.text.setPosition(position);
    textFields.push_back(textField);
}

void InformationWindow::addTextField(sf::Shape* shape, collision_detection::BoundingVolume* boundingVolume, float offsetX, float offsetY) {
    // Create a new text field
    TextField textField;
    textField.shape = shape;
    textField.boundingVolume = boundingVolume;
    textField.text.setFont(font);
    textField.text.setCharacterSize(12);
    textField.text.setFillColor(sf::Color::White);
    textField.offsetX = offsetX;
    textField.offsetY = offsetY;
    textFields.push_back(textField);
}
