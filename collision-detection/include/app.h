//
// Created by RINI on 20/09/2024.
//

#ifndef APP_H
#define APP_H
#include <iomanip>
#include <ios>
#include <string>

constexpr int WINDOW_DEFAULT_WIDTH = 1280; ///< Default width of the window.
constexpr int WINDOW_DEFAULT_HEIGHT = 1024; ///< Default height of the window.

/**
 * @brief Formats floating-point numbers to a string with two decimal places.
 * @param value The floating-point number to format.
 * @param precision The number of decimal places to include (default is 2).
 * @return A string representing the formatted number.
 */
inline std::string formatFloat(float value, int precision)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    return oss.str();
}

#endif //APP_H
