//
// Created by RINI on 25/09/2024.
//

#ifndef VISUALALGORITHM_H
#define VISUALALGORITHM_H

#include <coroutine>
#include <iostream>
#include <functional>
#include <memory>
#include <sys/stat.h>

#include "ei/2dtypes.hpp"
#include "SFML/Graphics.hpp"
#include "app.h"

#define RETURN_TYPE         std::shared_ptr<Visual>
#define INPUT_PARAMETER     std::vector<ei::Vec2>

struct IndicatorLine
{
    sf::Text text;
    sf::VertexArray line;
    IndicatorLine()
    {
        line = sf::VertexArray(sf::LineStrip, 0);
    }
    IndicatorLine(const sf::Vector2f& start, const sf::Vector2f& end, const sf::Color& color) : IndicatorLine()
    {
        line.append(sf::Vertex(start, color));
        line.append(sf::Vertex(end, color));
    }
    void setLine(const sf::Vector2f& start, const sf::Vector2f& end, const sf::Color& color)
    {
        line.clear();
        line.append(sf::Vertex(start, color));
        line.append(sf::Vertex(end, color));
    }
    void setText(const std::string& text, const sf::Vector2f& position)
    {
        this->text.setString(text);
        this->text.setPosition(position);
    }
    void setFont(const sf::Font& font)
    {
        text.setFont(font);
    }
    void draw(sf::RenderWindow& window) const
    {
        window.draw(line);
        window.draw(text);
    }
};

struct Highlight
{
    sf::Text text;
    sf::CircleShape highlight;
    sf::Vector2f position;
    Highlight() = default;
    Highlight(const sf::Vector2f& position, const std::string& text, const sf::Color& color)
    {
        this->text.setCharacterSize(12);
        this->text.setFillColor(sf::Color::White);
        this->text.setPosition(position.x, position.y + 30); // 30 == offsetY
        this->text.setString(text);

        highlight.setRadius(5);
        highlight.setFillColor(color);
        highlight.setPosition(position);
        // Set origin to center
        highlight.setOrigin(5, 5);
        this->position = position;
    }
    void setFont(const sf::Font& font)
    {
        text.setFont(font);
    }
    void draw(sf::RenderWindow& window) const
    {
        window.draw(highlight);
        window.draw(text);
    }
};

struct Visual
{
    sf::Text explanation;
    sf::VertexArray current_hull;
    std::vector<IndicatorLine> indicator_lines;
    std::vector<Highlight> highlights;
    bool finished = false;
    Visual()
    {
        explanation.setCharacterSize(12);
        explanation.setFillColor(sf::Color::White);
        explanation.setPosition(10, 10); // top-left
        current_hull = sf::VertexArray(sf::LineStrip, 0);
        indicator_lines = std::vector<IndicatorLine>();
    }
    void setFont(const sf::Font& font)
    {
        explanation.setFont(font);
        for (auto& highlight : highlights)
        {
            highlight.setFont(font);
        }
        for (auto& line : indicator_lines)
        {
            line.setFont(font);
        }
    }
    void setExplanation(const std::string& text)
    {
        explanation.setString(text);
    }
    void addHighlight(const sf::Vector2f& position, const std::string& text, const sf::Color& color)
    {
        highlights.emplace_back(position, text, color);
    }
    void addIndicatorLine(const sf::Vector2f& start, const sf::Vector2f& end, const sf::Color& color)
    {
        indicator_lines.emplace_back(start, end, color);
    }
    void addHullPoint(const sf::Vector2f& position, const sf::Color& color = sf::Color::Red)
    {
        current_hull.append(sf::Vertex(position, color));
    }
    void removeHighlight(const sf::Vector2f& position)
    {
        std::erase_if(highlights, [position](const Highlight& highlight) {
            return highlight.position == position;
        });
    }
    void removeIndicatorLine(const sf::Vector2f& start, const sf::Vector2f& end)
    {
        std::erase_if(indicator_lines, [start, end](const IndicatorLine& indicator_line)
        {
            return indicator_line.line[0].position.x == start.x
            && indicator_line.line[0].position.y == start.y
            && indicator_line.line[1].position.x == end.x
            && indicator_line.line[1].position.y == end.y;
        });
    }
    void clearHighlights()
    {
        highlights.clear();
    }
    void clearIndicatorLines()
    {
        indicator_lines.clear();
    }
    void clearHullPoints()
    {
        current_hull.clear();
    }
    void draw(sf::RenderWindow& window) const
    {
        window.draw(explanation);
        window.draw(current_hull);
        for (auto& highlight : highlights)
        {
            highlight.draw(window);
        }
        for (auto& line : indicator_lines)
        {
            line.draw(window);
        }
    }
};

// Generator coroutine that yields integers (i.e. the generator template used)
struct AlgorithmGenerator {
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type {
        RETURN_TYPE current_value;
        std::suspend_always yield_value(RETURN_TYPE value) {
            current_value = std::move(value); // Move the shared_ptr to avoid copying
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

    RETURN_TYPE next() {
        if (!coro) {
            throw std::runtime_error("Attempt to use an invalid coroutine");
        }
        std::cout << "Resuming coroutine (State: " << coro.done() << ")" << std::endl;
        std::cout << "Address of coroutine: " << coro.address() << std::endl;
        coro.resume();
        return coro.promise().current_value;
    }
};

// inline AlgorithmGenerator exampleAlgorithm(const INPUT_PARAMETER& vector) // Passing data by value (copy)
// {
//     std::cout << "COROUTINE: " << std::to_string(vector.size()) << " points received." << std::endl;
//     for (const auto& point : vector)
//     {
//         std::cout << "COROUTINE: " << "Processing point: (" << point.x << "," << point.y << ")" << std::endl;
//         co_yield point;
//     }
//     co_return;
// }

class VisualAlgorithm {
public:
    VisualAlgorithm() = default;
    VisualAlgorithm(const std::function<AlgorithmGenerator(INPUT_PARAMETER&)>& generator_func);

    void setFont(const sf::Font& font);
    void setInput(const INPUT_PARAMETER& input_data);
    void draw(sf::RenderWindow& window) const;
    void visualStep();
    RETURN_TYPE step();
    void runAlgorithm(sf::RenderWindow& window);
    void reset();

    bool IsStarted() const { return started_; }

private:
    std::function<AlgorithmGenerator(INPUT_PARAMETER&)> algorithm_ = nullptr; // Stores the algorithm generator function
    AlgorithmGenerator generator_{nullptr}; // Stores the generator coroutine
    INPUT_PARAMETER input_data_;
    bool started_ = false;
    RETURN_TYPE visualization_ = nullptr;

    sf::Font font_;
};



#endif //VISUALALGORITHM_H
