//
// Created by RINI on 25/09/2024.
//

#include "VisualAlgorithm.h"

VisualAlgorithm::VisualAlgorithm(const std::function<AlgorithmGenerator(INPUT_PARAMETER&)>& generator_func)
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

void VisualAlgorithm::draw(sf::RenderWindow& window) const
{
    if(started_ && visualization_)
    {
        visualization_->draw(window);
    }
}

void VisualAlgorithm::visualStep()
{
    // Depending on algorithm, expect different meaning for return value and handle accordingly
    RETURN_TYPE visual = step();
    if (visual)
    {
        std::cout << "Step '" << visual->explanation.getString().toAnsiString() << "' completed." << std::endl;
        visual->setFont(font_);
        visualization_ = visual;
    }
    else
    {
        std::cerr << "Error: No visualization data returned from algorithm step." << std::endl;
    }
}

RETURN_TYPE VisualAlgorithm::step()
{
    if (!started_) {
        // Initialize the coroutine only once, and allow it to enter its first suspension point
        generator_ = algorithm_(input_data_);
        started_ = true;

        // Immediately move the coroutine to its first `co_yield` and set up for resumption
        if (!generator_.coro || generator_.coro.done()) {
            std::cerr << "Error: Coroutine initialization failed or it is already done.\n";
            return nullptr;  // Return a default value
        }

        // Resume it to hit the first `co_yield` statement
        generator_.coro.resume();
        return generator_.coro.promise().current_value;
    }

    // Check if the coroutine is still valid before resuming
    if (!generator_.coro || generator_.coro.done()) {
        std::cerr << "Error: Attempt to resume an invalid or completed coroutine.\n";
        return nullptr;  // Return a default value
    }

    // Call next() on the stored generator to get the next value
    return generator_.next();
}

void VisualAlgorithm::runAlgorithm(sf::RenderWindow& window)
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
        visualStep();
        draw(window);
    }
}

void VisualAlgorithm::reset()
{
    if (started_) {
        // The destructor of AlgorithmGenerator will destroy the coroutine
        generator_ = {}; // Reset the generator, destroying the old coroutine
        started_ = false;
    }
}
