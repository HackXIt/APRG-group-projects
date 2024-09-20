//
// Created by RINI on 20/09/2024.
//

#include "ConfigurationWindow.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

ConfigurationWindow::ConfigurationWindow(sf::Font& font, CircularMotion& circularMotion)
    : circularMotion(circularMotion), font(font), currentEditingField(EditingField::None)
{
    // Initialize input strings with current values
    radiusInput = std::to_string(circularMotion.getRadius());
    centerXInput = std::to_string(circularMotion.getCenter().x);
    centerYInput = std::to_string(circularMotion.getCenter().y);
    angleInput = std::to_string(circularMotion.getAngle());
    angularSpeedInput = std::to_string(circularMotion.getAngularSpeed());
    angularSpeedIncrement = std::to_string(circularMotion.getAngularSpeedIncrement());
    isClockwiseFlag = circularMotion.IsClockwise() ? "true" : "false";

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
                break;
            }
        }
    }

    // Handle keyboard input for editing properties
    if (event.type == sf::Event::TextEntered)
    {
        if (event.text.unicode == '\b') // Handle backspace
        {
            switch (currentEditingField)
            {
            case EditingField::Radius:
                if (!radiusInput.empty())
                    radiusInput.pop_back();
                break;
            case EditingField::CenterX:
                if (!centerXInput.empty())
                    centerXInput.pop_back();
                break;
            case EditingField::CenterY:
                if (!centerYInput.empty())
                    centerYInput.pop_back();
                break;
            case EditingField::Angle:
                if (!angleInput.empty())
                    angleInput.pop_back();
                break;
            case EditingField::AngularSpeed:
                if (!angularSpeedInput.empty())
                    angularSpeedInput.pop_back();
                break;
            case EditingField::AngularSpeedIncrement:
                if (!angularSpeedIncrement.empty())
                    angularSpeedIncrement.pop_back();
            default:
                break;
            }
        }
        else if ((event.text.unicode >= '0' && event.text.unicode <= '9') ||
                    event.text.unicode == '.') // Handle character input
        {
            char enteredChar = static_cast<char>(event.text.unicode);
            switch (currentEditingField)
            {
            case EditingField::Radius:
                radiusInput += enteredChar;
                break;
            case EditingField::CenterX:
                centerXInput += enteredChar;
                break;
            case EditingField::CenterY:
                centerYInput += enteredChar;
                break;
            case EditingField::Angle:
                angleInput += enteredChar;
                break;
            case EditingField::AngularSpeed:
                angularSpeedInput += enteredChar;
                break;
            case EditingField::AngularSpeedIncrement:
                angularSpeedIncrement += enteredChar;
            default:
                break;
            }
        }
    }

    // Keyboard inputs
    if (event.type == sf::Event::KeyPressed)
    {
        switch (event.key.code)
        {
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
        default:
            break;
        }
    }
}

void ConfigurationWindow::applyChanges()
{
    try
    {
        switch (currentEditingField)
        {
        case EditingField::Radius:
            if (!radiusInput.empty())
            {
                circularMotion.setRadius(std::stof(radiusInput));
                // Update the input string to reflect formatted value
                radiusInput = formatFloat(circularMotion.getRadius());
            }
            currentEditingField = EditingField::None;
            break;
        case EditingField::CenterX:
            {
                if (!centerXInput.empty())
                {
                    sf::Vector2f center = circularMotion.getCenter();
                    center.x = std::stof(centerXInput);
                    circularMotion.setCenter(center);
                    centerXInput = formatFloat(center.x);
                }
                currentEditingField = EditingField::None;
                break;
            }
        case EditingField::CenterY:
            {
                if (!centerYInput.empty())
                {
                    sf::Vector2f center = circularMotion.getCenter();
                    center.y = std::stof(centerYInput);
                    circularMotion.setCenter(center);
                    centerYInput = formatFloat(center.y);
                }
                currentEditingField = EditingField::None;
                break;
            }
        case EditingField::Angle:
            if (!angleInput.empty())
            {
                circularMotion.setAngle(std::stof(angleInput));
                angleInput = formatFloat(circularMotion.getAngle());
            }
            currentEditingField = EditingField::None;
            break;
        case EditingField::AngularSpeed:
            if (!angularSpeedInput.empty())
            {
                circularMotion.setAngularSpeed(std::stof(angularSpeedInput));
                angularSpeedInput = formatFloat(circularMotion.getAngularSpeed(), 5);
            }
            currentEditingField = EditingField::None;
            break;
        case EditingField::AngularSpeedIncrement:
            if (!angularSpeedIncrement.empty())
            {
                circularMotion.setAngularSpeedIncrement(std::stof(angularSpeedIncrement));
                angularSpeedIncrement = formatFloat(circularMotion.getAngularSpeedIncrement(), 5);
            }
            currentEditingField = EditingField::None;
            break;
        case EditingField::IsClockwise:
            circularMotion.toggleDirection();
            isClockwiseFlag = circularMotion.IsClockwise() ? "true" : "false";
            currentEditingField = EditingField::None;
            break;
        default:
            break;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Invalid input\n" << e.what();
    }
}

void ConfigurationWindow::draw(sf::RenderWindow& window)
{
    // Update the input strings if not currently editing
    if (currentEditingField != EditingField::Radius)
        radiusInput = formatFloat(circularMotion.getRadius());

    if (currentEditingField != EditingField::CenterX)
        centerXInput = formatFloat(circularMotion.getCenter().x);

    if (currentEditingField != EditingField::CenterY)
        centerYInput = formatFloat(circularMotion.getCenter().y);

    if (currentEditingField != EditingField::Angle)
        angleInput = formatFloat(circularMotion.getAngle(), 5);

    if (currentEditingField != EditingField::AngularSpeed)
        angularSpeedInput = formatFloat(circularMotion.getAngularSpeed(), 5);

    if (currentEditingField != EditingField::AngularSpeedIncrement)
        angularSpeedIncrement = formatFloat(circularMotion.getAngularSpeedIncrement(), 5);

    if (currentEditingField != EditingField::IsClockwise)
        isClockwiseFlag = circularMotion.IsClockwise() ? "true" : "false";

    // Update text positions and bounds
    updateTextPositions();

    // Highlight the selected field
    for (TextField& textField : textFields)
    {
        if (textField.field == currentEditingField)
            textField.text.setFillColor(sf::Color::Red);
        else
            textField.text.setFillColor(sf::Color::Black);
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
            textField.text.setString("Radius: " + radiusInput);
            break;
        case EditingField::CenterX:
            textField.text.setString("Center X: " + centerXInput);
            break;
        case EditingField::CenterY:
            textField.text.setString("Center Y: " + centerYInput);
            break;
        case EditingField::Angle:
            textField.text.setString("Angle: " + angleInput);
            break;
        case EditingField::AngularSpeed:
            textField.text.setString("Angular Speed: " + angularSpeedInput);
            break;
        case EditingField::AngularSpeedIncrement:
            textField.text.setString("Speed Increment: " + angularSpeedIncrement);
            break;
        case EditingField::IsClockwise:
            textField.text.setString("Clockwise: " + isClockwiseFlag);
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

std::string ConfigurationWindow::formatFloat(float value, int precision)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    return oss.str();
}