//
// Created by RINI on 20/09/2024.
//

#ifndef CONFIGURATIONWINDOW_H
#define CONFIGURATIONWINDOW_H


#include <functional>
#include <SFML/Graphics.hpp>
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
 enum class EditingField {Radius, CenterX, CenterY, Angle, AngularSpeed, AngularSpeedIncrement, IsClockwise, None };

 // Struct to hold text field information
 struct TextField
 {
  sf::Text text;
  EditingField field;
  sf::FloatRect bounds;
 };

 // Struct to hold input field information
 struct InputField
 {
  std::string label;
  std::string input;
  EditingField field;
  bool isNumeric;
  int precision;
  std::function<void(const std::string&)> setter;
  std::function<std::string()> getter;
 };

 EditingField currentEditingField; ///< Currently selected field for editing.
 InputField* currentInputField;         ///< Pointer to the current text input.

 std::vector<InputField> textInputs; ///< Vector of text input strings.
 std::vector<TextField> textFields;  ///< Vector of text fields for editing.

 // Configuration window rectangle
 sf::RectangleShape configWindowRect;

 /**
  * @brief Applies changes to the CircularMotion object based on user input.
  */
 void applyChanges();

 /**
  * @brief Updates the positions and bounds of the text fields.
  */
 void updateTextPositions();
};
#endif //CONFIGURATIONWINDOW_H
