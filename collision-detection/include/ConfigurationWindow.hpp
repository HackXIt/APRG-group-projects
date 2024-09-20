//
// Created by RINI on 20/09/2024.
//

#ifndef CONFIGURATIONWINDOW_H
#define CONFIGURATIONWINDOW_H


#include <../../cmake-build-debug/_deps/sfml-src/include/SFML/Graphics.hpp>
#include "CircularMotion.hpp"

/**
 * @class ConfigurationWindow
 * @brief Handles the configuration interface for editing CircularMotion properties.
 *
 * This class encapsulates the logic for displaying and editing the properties
 * of a CircularMotion object within a separate configuration window.
 */
class ConfigurationWindow
{
public:
 /**
  * @brief Constructor for ConfigurationWindow.
  * @param font Reference to the SFML font used for rendering text.
  * @param circularMotion Reference to the CircularMotion object to configure.
  */
 ConfigurationWindow(sf::Font& font, CircularMotion& circularMotion);

 /**
  * @brief Handles SFML events related to the configuration window.
  * @param event The SFML event to handle.
  * @param window Reference to the main SFML window.
  */
 void handleEvent(const sf::Event& event, const sf::RenderWindow& window);

 /**
  * @brief Draws the configuration window onto the main window.
  * @param window Reference to the main SFML window.
  */
 void draw(sf::RenderWindow& window);

private:
 CircularMotion& circularMotion;   ///< Reference to the CircularMotion object.
 sf::Font& font;                   ///< Reference to the SFML font for text rendering.

 // Enum for editing fields
 enum class EditingField { None, Radius, CenterX, CenterY, Angle, AngularSpeed, AngularSpeedIncrement, IsClockwise };
 EditingField currentEditingField; ///< Currently selected field for editing.

 // Input strings for editing
 std::string radiusInput;
 std::string centerXInput;
 std::string centerYInput;
 std::string angleInput;
 std::string angularSpeedInput;
 std::string angularSpeedIncrement;
 std::string isClockwiseFlag;

 // Struct to hold text field information
 struct TextField
 {
  sf::Text text;
  EditingField field;
  sf::FloatRect bounds;
 };

 std::vector<TextField> textFields;

 // Configuration window rectangle
 sf::RectangleShape configWindowRect;

 /**
  * @brief Applies changes to the CircularMotion object based on user input.
  */
 void applyChanges();

 /**
     * @brief Formats floating-point numbers to a string with two decimal places.
     * @param value The floating-point number to format.
     * @param precision The number of decimal places to include (default is 2).
     * @return A string representing the formatted number.
     */
 static std::string formatFloat(float value, int precision = 2);

 /**
  * @brief Updates the positions and bounds of the text fields.
  */
 void updateTextPositions();
};
#endif //CONFIGURATIONWINDOW_H
