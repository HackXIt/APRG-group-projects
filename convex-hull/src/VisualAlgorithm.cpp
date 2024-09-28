//
// Created by RINI on 25/09/2024.
//

#include "VisualAlgorithm.h"

#include "SFML/Graphics.hpp"

VisualAlgorithm::VisualAlgorithm(const std::function<AlgorithmGenerator(const INPUT_PARAMETER&)>& generator_func)
{
    algorithm_ = generator_func;
}

void VisualAlgorithm::setInput(const INPUT_PARAMETER& input_data)
{
    input_data_ = input_data;
}

void VisualAlgorithm::setFont(const sf::Font& font)
{
    font_ = font;
}

void VisualAlgorithm::draw(sf::RenderWindow& window)
{
    for(const auto & visual : visuals_)
    {
        window.draw(visual.highlight);
        window.draw(visual.text);
    }
}

void VisualAlgorithm::visualStep()
{
    ei::Vec2 point = step();
    sf::CircleShape pointHighlight(5);
    pointHighlight.setPosition(point.x, point.y);
    visuals_.emplace_back(font_, pointHighlight.getPosition(), "Current point", sf::Color::Green);
}

ei::Vec2 VisualAlgorithm::step()
{
    if (!started_) {
        // Initialize the coroutine only once, and allow it to enter its first suspension point
        generator_ = algorithm_(input_data_);
        started_ = true;

        // Immediately move the coroutine to its first `co_yield` and set up for resumption
        if (!generator_.coro || generator_.coro.done()) {
            std::cerr << "Error: Coroutine initialization failed or it is already done.\n";
            return ei::Vec2(0, 0);
        }

        // Resume it to hit the first `co_yield` statement
        generator_.coro.resume();
        return generator_.coro.promise().current_value;
    }

    // Check if the coroutine is still valid before resuming
    if (!generator_.coro || generator_.coro.done()) {
        std::cerr << "Error: Attempt to resume an invalid or completed coroutine.\n";
        return {0, 0};  // Return a default value
    }

    // Call next() on the stored generator to get the next value
    return generator_.next();
}

void VisualAlgorithm::runAlgorithm()
{
    if (started_)
    {
        std::cerr << "Algorithm already started. Please reset the algorithm to start again.\n";
        return;
    }
    generator_ = algorithm_(input_data_);
    started_ = true;
    // ei::Vec2 first = generator_.coro.promise().current_value;
    // std::cout << "Received 1st point: (" << first.x << "," << first.y << ")" << std::endl;
    while (!generator_.coro.done())
    {
        ei::Vec2 point = generator_.next();
        std::cout << "Received point: (" << point.x << "," << point.y << ")" << std::endl;
    }
}

void VisualAlgorithm::reset()
{
    if (started_) {
        // The destructor of AlgorithmGenerator will destroy the coroutine
        generator_ = {}; // Reset the generator, destroying the old coroutine
        visuals_.clear(); // Clear any existing visuals
        started_ = false;
    }
}
