//
// Created by RINI on 20/09/2024.
//

#include "ConfigurationWindow.hpp"

#include <functional>
#include <iostream>
#include <sstream>
#include <iomanip>

ConfigurationWindow::ConfigurationWindow(sf::Font& font, CircularMotion* circularMotion, LinearMotion* linearMotion)
    : circularMotion(circularMotion), linearMotion(linearMotion), font(font), currentEditingField(EditingField::None), currentInputField(nullptr)
{
    // !! (KEEP THE ORDER THE SAME AS IN THE EditingField ENUM) !!
    // Initialize input fields with default values
    // NOTE : The setter and getter are lambda functions handling the conversion for the fields (e.g., string to float and back)
#pragma region InputFields
    if(circularMotion != nullptr)
    {
        textInputs = {
            { "Radius: ",
                std::to_string(circularMotion->getRadius()),
                EditingField::Radius,
                true,
                2,
                [circularMotion](const std::string& val) {
                    circularMotion->setRadius(std::stof(val));
            },
            [this, circularMotion]() {
                return formatFloat(circularMotion->getRadius(), 2);
            }
            },
            { "Center X: ",
                std::to_string(circularMotion->getCenter().x),
                EditingField::CenterX,
                true,
                0,
                [circularMotion](const std::string& val) {
                    sf::Vector2f center = circularMotion->getCenter();
                    center.x = std::stof(val);
                    circularMotion->setCenter(center);
            },
            [this, circularMotion]() {
                return formatFloat(circularMotion->getCenter().x, 0);
            }
            },
            { "Center Y: ",
                std::to_string(circularMotion->getCenter().y),
                EditingField::CenterY,
                true,
                0,
                [circularMotion](const std::string& val) {
                    sf::Vector2f center = circularMotion->getCenter();
                    center.y = std::stof(val);
                    circularMotion->setCenter(center);
            },
            [this, circularMotion]() {
                return formatFloat(circularMotion->getCenter().y, 0);
            }
            },
            { "Angle: ",
                std::to_string(circularMotion->getAngle()),
                EditingField::Angle,
                true,
                2,
                [circularMotion](const std::string& val) {
                    circularMotion->setAngle(std::stof(val));
            },
            [this, circularMotion]() {
                return formatFloat(circularMotion->getAngle(), 2);
            }
            },
            { "Angular Speed: ",
                std::to_string(circularMotion->getAngularSpeed()),
                EditingField::AngularSpeed,
                true,
                5,
                [circularMotion](const std::string& val) {
                    circularMotion->setAngularSpeed(std::stof(val));
            },
            [this, circularMotion]() {
                return formatFloat(circularMotion->getAngularSpeed(), 5);
            }
            },
            { "Speed Increment: ",
                std::to_string(circularMotion->getAngularSpeedIncrement()),
                EditingField::AngularSpeedIncrement,
                true,
                5,
                [circularMotion](const std::string& val) {
                    circularMotion->setAngularSpeedIncrement(std::stof(val));
            },
            [this, circularMotion]() {
                return formatFloat(circularMotion->getAngularSpeedIncrement(), 5);
            }
            },
            { "Clockwise: ",
                circularMotion->IsClockwise() ? "true" : "false",
                EditingField::IsClockwise,
                false,
                -1,
                [circularMotion](const std::string& val) {
                    if (val == "true")
                        circularMotion->setClockwise();
                    else if (val == "false")
                        circularMotion->setCounterClockwise();
            },
            [circularMotion]() {
                return circularMotion->IsClockwise() ? "true" : "false";
            }
            }
        };
        textFields = {
            { sf::Text("", font, 16), EditingField::Radius, static_cast<int>(EditingFieldCircular::Radius), {} },
            { sf::Text("", font, 16), EditingField::CenterX, static_cast<int>(EditingFieldCircular::CenterX), {} },
            { sf::Text("", font, 16), EditingField::CenterY, static_cast<int>(EditingFieldCircular::CenterY), {} },
            { sf::Text("", font, 16), EditingField::Angle, static_cast<int>(EditingFieldCircular::Angle), {} },
            { sf::Text("", font, 16), EditingField::AngularSpeed, static_cast<int>(EditingFieldCircular::AngularSpeed), {} },
            { sf::Text("", font, 16), EditingField::AngularSpeedIncrement, static_cast<int>(EditingFieldCircular::AngularSpeedIncrement), {} },
            { sf::Text("", font, 16), EditingField::IsClockwise, static_cast<int>(EditingFieldCircular::IsClockwise), {} }
        };
    } else if (linearMotion != nullptr)
    {
        textInputs = {
            {"Velocity: ",
                std::to_string(linearMotion->getVelocity()),
                EditingField::Velocity,
                true,
                2,
                [linearMotion](const std::string& val)
                {
                    linearMotion->setVelocity(std::stof(val));
                },
                [this, linearMotion]()
                {
                    return formatFloat(linearMotion->getVelocity(), 5);
                }
            },
            {"TargetX: ",
                std::to_string(linearMotion->getTarget().x),
                EditingField::TargetX,
                true,
                2,
                [linearMotion](const std::string& val)
                {
                    sf::Vector2f target = linearMotion->getTarget();
                    target.x = std::stof(val);
                    linearMotion->setTarget(target);
                },
                [this, linearMotion]()
                {
                    sf::Vector2f target = linearMotion->getTarget();
                    return "X: " + formatFloat(target.x, 2);
                }
            },
            {"TargetY: ",
                std::to_string(linearMotion->getTarget().y),
                EditingField::TargetX,
                true,
                2,
                [linearMotion](const std::string& val)
                {
                    sf::Vector2f target = linearMotion->getTarget();
                    target.y = std::stof(val);
                    linearMotion->setTarget(target);
                },
                [this, linearMotion]()
                {
                    sf::Vector2f target = linearMotion->getTarget();
                    return "Y: " + formatFloat(target.y, 2);
                }
            }
        };
        textFields = {
            { sf::Text("", font, 16), EditingField::Velocity, static_cast<int>(EditingFieldLinear::Velocity), {} },
            { sf::Text("", font, 16), EditingField::TargetX, static_cast<int>(EditingFieldLinear::TargetX), {} },
            { sf::Text("", font, 16), EditingField::TargetY, static_cast<int>(EditingFieldLinear::TargetY), {} }
        };
    }
#pragma endregion
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
                currentInputField = &textInputs[textField.fieldIndex];
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
            //textInputs[static_cast<int>(currentEditingField)].input = currentInputField->input;
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
            if(circularMotion != nullptr)
                circularMotion->setCounterClockwise();
            break;
        case sf::Keyboard::Right:   // Change direction of rotation to clockwise
            if(circularMotion != nullptr)
                circularMotion->setClockwise();
            break;
        case sf::Keyboard::Up:    // Increase angular speed
            if(circularMotion != nullptr)
                circularMotion->setAngularSpeed(circularMotion->getAngularSpeed() + circularMotion->getAngularSpeedIncrement());
            if(linearMotion != nullptr)
                linearMotion->setVelocity(linearMotion->getVelocity() + 0.01f);
            break;
        case sf::Keyboard::Down:    // Decrease angular speed (minimum 0.001f)
            if (circularMotion != nullptr && circularMotion->getAngularSpeed() > circularMotion->getAngularSpeedIncrement())
                circularMotion->setAngularSpeed(circularMotion->getAngularSpeed() - circularMotion->getAngularSpeedIncrement());
            if (linearMotion != nullptr && linearMotion->getVelocity() > 0)
                linearMotion->setVelocity(linearMotion->getVelocity() - 0.01f);
            break;
        case sf::Keyboard::W:   // Move up in the Y-axis
            if (circularMotion != nullptr && (circularMotion->getCenter().y - circularMotion->getRadius()) > 0)
                circularMotion->setCenter(circularMotion->getCenter() + sf::Vector2f(0.f, -5.f));
            if (linearMotion != nullptr)
                linearMotion->moveShape(0, -1.f);
            break;
        case sf::Keyboard::S:   // Move down in the Y-axis
            if (circularMotion != nullptr && (circularMotion->getCenter().y + circularMotion->getRadius()) < window.getSize().y)
                circularMotion->setCenter(circularMotion->getCenter() + sf::Vector2f(0.f, 5.f));
            if (linearMotion != nullptr)
                linearMotion->moveShape(0, 1.f);
            break;
        case sf::Keyboard::A:   // Move left in the X-axis
            if (circularMotion != nullptr && (circularMotion->getCenter().x - circularMotion->getRadius()) > 0)
                circularMotion->setCenter(circularMotion->getCenter() + sf::Vector2f(-5.f, 0.f));
            if (linearMotion != nullptr)
                linearMotion->moveShape(-1.f, 0);
            break;
        case sf::Keyboard::D:   // Move right in the X-axis
            if (circularMotion != nullptr && (circularMotion->getCenter().x + circularMotion->getRadius()) < window.getSize().x)
                circularMotion->setCenter(circularMotion->getCenter() + sf::Vector2f(5.f, 0.f));
            if (linearMotion != nullptr)
                linearMotion->moveShape(1.f, 0);
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
            textField.text.setString("Radius: " + textInputs[static_cast<int>(EditingFieldCircular::Radius)].input);
            break;
        case EditingField::CenterX:
            textField.text.setString("Center X: " + textInputs[static_cast<int>(EditingFieldCircular::CenterX)].input);
            break;
        case EditingField::CenterY:
            textField.text.setString("Center Y: " + textInputs[static_cast<int>(EditingFieldCircular::CenterY)].input);
            break;
        case EditingField::Angle:
            textField.text.setString("Angle: " + textInputs[static_cast<int>(EditingFieldCircular::Angle)].input);
            break;
        case EditingField::AngularSpeed:
            textField.text.setString("Angular Speed: " + textInputs[static_cast<int>(EditingFieldCircular::AngularSpeed)].input);
            break;
        case EditingField::AngularSpeedIncrement:
            textField.text.setString("Speed Increment: " + textInputs[static_cast<int>(EditingFieldCircular::AngularSpeedIncrement)].input);
            break;
        case EditingField::IsClockwise:
            textField.text.setString("Clockwise: " + textInputs[static_cast<int>(EditingFieldCircular::IsClockwise)].input);
            break;
        case EditingField::Velocity:
            textField.text.setString("Velocity: " + textInputs[static_cast<int>(EditingFieldLinear::Velocity)].input);
            break;
        case EditingField::TargetX:
            textField.text.setString("TargetX: " + textInputs[static_cast<int>(EditingFieldLinear::TargetX)].input);
            break;
        case EditingField::TargetY:
            textField.text.setString("TargetY: " + textInputs[static_cast<int>(EditingFieldLinear::TargetY)].input);
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