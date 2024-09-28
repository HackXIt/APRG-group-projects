//
// Created by RINI on 25/09/2024.
//

#include "TextWindow.h"

TextWindow::TextWindow(sf::Font& font) : font(font) {
}

void TextWindow::updateInformation() {
    // Update the text positions
    for(auto& textField : textFields) {
        textField.field.setString(textField.text);
    }
}

void TextWindow::draw(sf::RenderWindow& window) {

    updateInformation();

    for(auto& textField : textFields) {
        window.draw(textField.field);
    }
}

int TextWindow::addTextField(sf::Vector2f position, float offsetX, float offsetY){
    // Create a new text field
    TextField textField;
    textField.field.setFont(font);
    textField.field.setCharacterSize(12);
    textField.field.setFillColor(sf::Color::White);
    textField.field.setPosition(position);
    textFields.push_back(textField);
    return textFields.size() - 1; // return added index
}

void TextWindow::setText(int textFieldIndex, const std::string &text)
{
    textFields[textFieldIndex].text = text;
}

void TextWindow::updatePosition(int textFieldIndex, sf::Vector2f position, float offsetX, float offsetY)
{
    textFields[textFieldIndex].coords = position;
    textFields[textFieldIndex].offsetX = offsetX;
    textFields[textFieldIndex].offsetY = offsetY;
}

