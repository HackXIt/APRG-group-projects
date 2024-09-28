//
// Created by RINI on 25/09/2024.
//

#ifndef VISUALALGORITHM_H
#define VISUALALGORITHM_H

#include <coroutine>
#include <iostream>
#include <functional>

#include "ei/2dtypes.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"

#define INPUT_PARAMETER     std::vector<ei::Vec2>

// Generator coroutine that yields integers (i.e. the generator template used)
struct AlgorithmGenerator {
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type {
        ei::Vec2 current_value;
        std::suspend_always yield_value(const ei::Vec2 value) {
            current_value = value;
            return {};
        }
        AlgorithmGenerator get_return_object() {
            return AlgorithmGenerator{handle_type::from_promise(*this)};
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {
            std::exit(1);
        }
    };

    handle_type coro = nullptr;

    AlgorithmGenerator() = default;

    AlgorithmGenerator(handle_type h) : coro(h) {}
    AlgorithmGenerator(const AlgorithmGenerator&) = delete; // Disable copying
    AlgorithmGenerator& operator=(const AlgorithmGenerator&) = delete; // Disable copy assignment

    // Move constructor
    AlgorithmGenerator(AlgorithmGenerator&& other) noexcept : coro(other.coro) {
        other.coro = nullptr;
    }
    // Move assignment operator
    AlgorithmGenerator& operator=(AlgorithmGenerator&& other) noexcept {
        if (this != &other) {
            if (coro) {
                coro.destroy();
            }
            coro = other.coro;
            other.coro = nullptr;
        }
        return *this;
    }

    ~AlgorithmGenerator() {
        if (coro) {
            coro.destroy();
        }
    }

    bool done() const { return coro.done(); }

    ei::Vec2 next() {
        if (!coro) {
            throw std::runtime_error("Attempt to use an invalid coroutine");
        }
        std::cout << "Resuming coroutine (State: " << coro.done() << ")" << std::endl;
        std::cout << "Address of coroutine: " << coro.address() << std::endl;
        coro.resume();
        return coro.promise().current_value;
    }
};

inline AlgorithmGenerator exampleAlgorithm(const INPUT_PARAMETER& vector) // Passing data by value (copy)
{
    std::cout << "COROUTINE: " << std::to_string(vector.size()) << " points received." << std::endl;
    for (const auto& point : vector)
    {
        std::cout << "COROUTINE: " << "Processing point: (" << point.x << "," << point.y << ")" << std::endl;
        co_yield point;
    }
    co_return;
}

struct Visual
{
    sf::Text text;
    sf::CircleShape highlight;
    sf::Vector2f position;
    Visual(const sf::Font& font, const sf::Vector2f& position, const std::string& text, const sf::Color& color)
    {
        this->text.setFont(font);
        this->text.setCharacterSize(12);
        this->text.setFillColor(sf::Color::White);
        this->text.setPosition(position.x, position.y + 25); // 25 == offsetY
        this->text.setString(text);

        highlight.setRadius(5);
        highlight.setFillColor(color);
        highlight.setPosition(position);
        this->position = position;
    }
};

class VisualAlgorithm {
public:
    VisualAlgorithm() = default;
    VisualAlgorithm(const std::function<AlgorithmGenerator(const INPUT_PARAMETER&)>& generator_func);

    void setFont(const sf::Font& font);
    void setInput(const INPUT_PARAMETER& input_data);
    void draw(sf::RenderWindow& window);
    void visualStep();
    ei::Vec2 step();
    void runAlgorithm();
    void reset();

    bool IsStarted() const { return started_; }

private:
    std::function<AlgorithmGenerator(const INPUT_PARAMETER&)> algorithm_ = nullptr; // Stores the algorithm generator function
    AlgorithmGenerator generator_{nullptr}; // Stores the generator coroutine
    INPUT_PARAMETER input_data_;
    bool started_ = false;
    std::vector<Visual> visuals_;
    sf::Font font_;
};



#endif //VISUALALGORITHM_H
