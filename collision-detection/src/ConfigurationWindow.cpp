//
// Created by RINI on 20/09/2024.
//

#include "ConfigurationWindow.hpp"

#include <functional>
#include <iostream>
#include <sstream>
#include <iomanip>

ConfigurationWindow::ConfigurationWindow(sf::Font& font, CircularMotion& circularMotion)
    : circularMotion(circularMotion), font(font), currentEditingField(EditingField::None), currentInputField(nullptr)
{
    // !! (KEEP THE ORDER THE SAME AS IN THE EditingField ENUM) !!
    // Initialize input fields with default values
    // NOTE : The setter and getter are lambda functions handling the conversion for the fields (e.g., string to float and back)
#pragma region InputFields
    textInputs = {
      { "Radius: ",
          std::to_string(circularMotion.getRadius()),
          EditingField::Radius,
          true,
          2,
          [&circularMotion](const std::string& val) {
              circularMotion.setRadius(std::stof(val));
          },
          [this, &circularMotion]() {
              return formatFloat(circularMotion.getRadius(), 2);
          }
      },
      { "Center X: ",
          std::to_string(circularMotion.getCenter().x),
          EditingField::CenterX,
          true,
          0,
          [&circularMotion](const std::string& val) {
              sf::Vector2f center = circularMotion.getCenter();
              center.x = std::stof(val);
              circularMotion.setCenter(center);
          },
          [this, &circularMotion]() {
              return formatFloat(circularMotion.getCenter().x, 0);
          }
      },
      { "Center Y: ",
          std::to_string(circularMotion.getCenter().y),
          EditingField::CenterY,
          true,
          0,
          [&circularMotion](const std::string& val) {
              sf::Vector2f center = circularMotion.getCenter();
              center.y = std::stof(val);
              circularMotion.setCenter(center);
          },
          [this, &circularMotion]() {
              return formatFloat(circularMotion.getCenter().y, 0);
          }
      },
      { "Angle: ",
          std::to_string(circularMotion.getAngle()),
          EditingField::Angle,
          true,
          2,
          [&circularMotion](const std::string& val) {
              circularMotion.setAngle(std::stof(val));
          },
          [this, &circularMotion]() {
              return formatFloat(circularMotion.getAngle(), 2);
          }
      },
      { "Angular Speed: ",
          std::to_string(circularMotion.getAngularSpeed()),
          EditingField::AngularSpeed,
          true,
          5,
          [&circularMotion](const std::string& val) {
              circularMotion.setAngularSpeed(std::stof(val));
          },
          [this, &circularMotion]() {
              return formatFloat(circularMotion.getAngularSpeed(), 5);
          }
      },
      { "Speed Increment: ",
          std::to_string(circularMotion.getAngularSpeedIncrement()),
          EditingField::AngularSpeedIncrement,
          true,
          5,
          [&circularMotion](const std::string& val) {
              circularMotion.setAngularSpeedIncrement(std::stof(val));
          },
          [this, &circularMotion]() {
              return formatFloat(circularMotion.getAngularSpeedIncrement(), 5);
          }
      },
      { "Clockwise: ",
          circularMotion.IsClockwise() ? "true" : "false",
          EditingField::IsClockwise,
          false,
          -1,
          [&circularMotion](const std::string& val) {
              if (val == "true")
                  circularMotion.setClockwise();
              else if (val == "false")
                  circularMotion.setCounterClockwise();
          },
          [&circularMotion]() {
              return circularMotion.IsClockwise() ? "true" : "false";
          }
      }
    };
#pragma endregion

    // Initialize text fields
    textFields = {
        { sf::Text("", font, 16), EditingField::Radius, {} },
        { sf::Text("", font, 16), EditingField::CenterX, {} },
        { sf::Text("", font, 16), EditingField::CenterY, {} },
        { sf::Text("", font, 16), EditingField::Angle, {} },
        { sf::Text("", font, 16), EditingField::AngularSpeed, {} },
        { sf::Text("", font, 16), EditingField::AngularSpeedIncrement, {} },
        { sf::Text("", font, 16), EditingField::IsClockwise, {} }
    };

    // Initialize configuration window rectangle
    configWindowRect.setFillColor(sf::Color(200, 200, 200, 200));
}

void ConfigurationWindow::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    // Ensure text positions and bounds are up to date
    updateTextPositions();

    if (event.type == sf::Event::MouseButtonPressed)
    {
        // Get mouse position
        sf::Vector2i mousePosWindow = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(mousePosWindow);

        // Check if mouse is within the bounds of the text inputs
        currentEditingField = EditingField::None;
        for (auto& textField : textFields)
        {
            if (textField.bounds.contains(mousePos))
            {
                currentEditingField = textField.field;
                currentInputField = &textInputs[static_cast<int>(currentEditingField)];
                currentInputField->input.clear(); // Start with an empty string
                break;
            }
        }
    }

    // Handle keyboard input for editing properties
    if (event.type == sf::Event::TextEntered && currentInputField != nullptr)
    {
        if (event.text.unicode == '\b') // Handle backspace
        {
            if (!currentInputField->input.empty())
                currentInputField->input.pop_back();
        }
        else if (((event.text.unicode >= '0' && event.text.unicode <= '9') ||
                    event.text.unicode == '.')) // Handle character input
        {
            char enteredChar = static_cast<char>(event.text.unicode);
            currentInputField->input += enteredChar;
            textInputs[static_cast<int>(currentEditingField)].input = currentInputField->input;
        }
    }

    // Keyboard inputs
    if (event.type == sf::Event::KeyPressed)
    {
        switch (event.key.code)
        {
        case sf::Keyboard::Escape: // Cancel editing
            currentEditingField = EditingField::None;
            currentInputField = nullptr;
            break;
        case sf::Keyboard::Enter: // Apply changes when pressing Enter
            applyChanges();
            break;
        case sf::Keyboard::Left:    // Change direction of rotation to counter-clockwise
            circularMotion.setCounterClockwise();
            break;
        case sf::Keyboard::Right:   // Change direction of rotation to clockwise
            circularMotion.setClockwise();
            break;
        case sf::Keyboard::Up:    // Increase angular speed
            circularMotion.setAngularSpeed(circularMotion.getAngularSpeed() + circularMotion.getAngularSpeedIncrement());
            break;
        case sf::Keyboard::Down:    // Decrease angular speed (minimum 0.001f)
            if (circularMotion.getAngularSpeed() > circularMotion.getAngularSpeedIncrement())
                circularMotion.setAngularSpeed(circularMotion.getAngularSpeed() - circularMotion.getAngularSpeedIncrement());
            break;
        case sf::Keyboard::W:   // Move up in the Y-axis
            if ((circularMotion.getCenter().y - circularMotion.getRadius()) > 0)
                circularMotion.setCenter(circularMotion.getCenter() + sf::Vector2f(0.f, -5.f));
            break;
        case sf::Keyboard::S:   // Move down in the Y-axis
            if ((circularMotion.getCenter().y + circularMotion.getRadius()) < window.getSize().y)
                circularMotion.setCenter(circularMotion.getCenter() + sf::Vector2f(0.f, 5.f));
            break;
        case sf::Keyboard::A:   // Move left in the X-axis
            if ((circularMotion.getCenter().x - circularMotion.getRadius()) > 0)
                circularMotion.setCenter(circularMotion.getCenter() + sf::Vector2f(-5.f, 0.f));
            break;
        case sf::Keyboard::D:   // Move right in the X-axis
            if ((circularMotion.getCenter().x + circularMotion.getRadius()) < window.getSize().x)
                circularMotion.setCenter(circularMotion.getCenter() + sf::Vector2f(5.f, 0.f));
            break;
        default:
            break;
        }
    }
}

void ConfigurationWindow::applyChanges()
{
    try
    {
        if (currentInputField == nullptr)
        {
            std::cerr << "No input field selected\n";
            return;
        }
        if (currentInputField->input.empty())
        {
            std::cerr << "Input is empty\n";
            return;
        }
        currentInputField->setter(currentInputField->input);
        currentEditingField = EditingField::None;
        currentInputField = nullptr;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Invalid input\n" << e.what();
    }
}

void ConfigurationWindow::draw(sf::RenderWindow& window)
{
    // Update the input strings if not currently editing
    for (auto& input : textInputs)
    {
        if(input.field == currentEditingField)
            continue;
        input.input = input.getter();
    }

    // Update text positions and bounds
    updateTextPositions();

    // Highlight the selected field
    for (TextField& textField : textFields)
    {
        if (textField.field == currentEditingField)
        {
            textField.text.setFillColor(sf::Color::Red);
        }
        else
        {
            textField.text.setFillColor(sf::Color::Black);
        }
    }

    // Draw the configuration window rectangle
    window.draw(configWindowRect);

    // Draw the texts
    for (auto& textField : textFields)
    {
        window.draw(textField.text);
    }
}

void ConfigurationWindow::updateTextPositions()
{
    // Update the texts
    for (auto& textField : textFields)
    {
        switch (textField.field)
        {
        case EditingField::Radius:
            textField.text.setString("Radius: " + textInputs[static_cast<int>(EditingField::Radius)].input);
            break;
        case EditingField::CenterX:
            textField.text.setString("Center X: " + textInputs[static_cast<int>(EditingField::CenterX)].input);
            break;
        case EditingField::CenterY:
            textField.text.setString("Center Y: " + textInputs[static_cast<int>(EditingField::CenterY)].input);
            break;
        case EditingField::Angle:
            textField.text.setString("Angle: " + textInputs[static_cast<int>(EditingField::Angle)].input);
            break;
        case EditingField::AngularSpeed:
            textField.text.setString("Angular Speed: " + textInputs[static_cast<int>(EditingField::AngularSpeed)].input);
            break;
        case EditingField::AngularSpeedIncrement:
            textField.text.setString("Speed Increment: " + textInputs[static_cast<int>(EditingField::AngularSpeedIncrement)].input);
            break;
        case EditingField::IsClockwise:
            textField.text.setString("Clockwise: " + textInputs[static_cast<int>(EditingField::IsClockwise)].input);
            break;
        default:
            break;
        }
    }

    // Position the texts
    float padding = 10.f;
    float yOffset = padding + 10.f; // Start below the top padding
    float maxWidth = 0.f;

    for (auto& textField : textFields)
    {
        textField.text.setPosition(padding + 10.f, yOffset);
        sf::FloatRect bounds = textField.text.getGlobalBounds();
        textField.bounds = bounds;

        if (bounds.width > maxWidth)
            maxWidth = bounds.width;

        yOffset += bounds.height + padding; // Add padding between texts
    }

    // Adjust the size of the configuration window rectangle
    configWindowRect.setPosition(padding, padding);
    configWindowRect.setSize(sf::Vector2f(maxWidth + 20.f, yOffset - padding));
    configWindowRect.setFillColor(sf::Color::Cyan);
}