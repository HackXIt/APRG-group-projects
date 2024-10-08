﻿//
// Created by RINI on 20/09/2024.
//

#ifndef APP_H
#define APP_H
#include <iomanip>
#include <ios>
#include <string>

constexpr int WINDOW_DEFAULT_WIDTH = 1280; ///< Default width of the window.
constexpr int WINDOW_DEFAULT_HEIGHT = 1024; ///< Default height of the window.

#define VISUALIZATION_POINTS_LIMIT 50

enum Algorithm
{
    QUICK_HULL = 0,
    JARVIS_MARCH = 1,
    DIVIDE_AND_CONQUER = 2,
    INVALID_ALGORITHM = 3
};

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

int console_main(Algorithm algorithm, std::vector<ei::Vec2>& loadedPoints);
// Using a pointer reference to allow for default nullptr value (i.e. no points loaded)
int gui_main(Algorithm algorithm, std::vector<ei::Vec2>* loadedPoints = nullptr);

inline std::string algorithmToString(Algorithm algorithm)
{
    switch (algorithm)
    {
    case QUICK_HULL:
        return "QuickHull";
    case JARVIS_MARCH:
        return "Jarvis March";
    case DIVIDE_AND_CONQUER:
        return "Divide & Conquer";
    default:
            return "Unknown";
    }
}

#endif //APP_H
