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

#define RETURN_TYPE         ei::Vec2
#define INPUT_PARAMETER     std::vector<ei::Vec2>

// Generator coroutine that yields integers (i.e. the generator template used)
struct AlgorithmGenerator {
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type {
        RETURN_TYPE current_value;
        std::suspend_always yield_value(const RETURN_TYPE value) {
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

inline int pointLocation(const ei::Vec2& A, const ei::Vec2& B, const ei::Vec2& P) {
    float cp1 = (B.x - A.x)*(P.y - A.y) - (B.y - A.y)*(P.x - A.x);
    if (cp1 > 0.0f)
        return 1;
    else if (cp1 == 0.0f)
        return 0;
    else
        return -1;
}

inline float distanceFromLine(const ei::Vec2& A, const ei::Vec2& B, const ei::Vec2& P) {
    float num = std::abs((B.y - A.y)*P.x - (B.x - A.x)*P.y + B.x*A.y - B.y*A.x);
    float den = std::sqrt((B.y - A.y)*(B.y - A.y) + (B.x - A.x)*(B.x - A.x)); //sqrt can be optimised
    return num / den;
}

#include <stack>


//P1 (Point1 left), P2 (Point2 right), C (New Point center)
inline AlgorithmGenerator quickHullAlgorithm(const INPUT_PARAMETER& points)
{
    if (points.size() <3) {
        for (const auto& point : points) {
            co_yield point;
        }
        co_return;
    }

    auto minmaxX = std::minmax_element(points.begin(), points.end(),
        [](const ei::Vec2& a, const ei::Vec2& b){return a.x < b.x; });

    ei::Vec2 A = *minmaxX.first;
    ei::Vec2 B = *minmaxX.second;

    co_yield A;
    co_yield B;

    std::vector<ei::Vec2> leftSet;
    std::vector<ei::Vec2> rightSet;

    for (const auto& point : points) {
        if (point == A || point == B) continue;
        if (pointLocation(A, B, point) == 1)
            leftSet.push_back(point);
        else if (pointLocation(A, B, point) == -1)
            rightSet.push_back(point);
    }

    //wegen yield wird rec nur simuliert.
    std::stack<std::tuple<ei::Vec2, ei::Vec2, std::vector<ei::Vec2>>> stack;

    std::vector<ei::Vec2> convexHull = {A, B};

    stack.push(std::make_tuple(A, B, leftSet));
    stack.push(std::make_tuple(B, A, rightSet));

    while (!stack.empty()) {
        auto [P1, P2, set] = stack.top();
        stack.pop();

        if (set.empty())
            continue;

        float maxDistance = -1.0f;
        ei::Vec2 C;

        for (const auto& point : set) {
            float distance = distanceFromLine(P1, P2, point);
            if (distance > maxDistance) {
                maxDistance = distance;
                C = point;
            }
        }

        co_yield C;

        auto it = std::find(convexHull.begin(), convexHull.end(), P2);
        convexHull.insert(it, C);


        std::vector<ei::Vec2> leftSet1;
        std::vector<ei::Vec2> leftSet2;

        for (const auto& point : set) {
            if (point == C)
                continue;
            if (pointLocation(P1, C, point) == 1)
                leftSet1.push_back(point);
            else if (pointLocation(C, P2, point) == 1)
                leftSet2.push_back(point);
        }

        stack.push(std::make_tuple(C, P2, leftSet2));
        stack.push(std::make_tuple(P1, C, leftSet1));
    }

    for (const auto& point : convexHull) {
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
