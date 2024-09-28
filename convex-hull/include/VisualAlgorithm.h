//
// Created by RINI on 25/09/2024.
//

#ifndef VISUALALGORITHM_H
#define VISUALALGORITHM_H

#include <coroutine>
#include <iostream>
#include <functional>
#include <SFML/Window/Window.hpp>

#include "ei/2dtypes.hpp"

// Generator coroutine that yields integers (i.e. the generator template used)
struct AlgorithmGenerator {
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type {
        ei::Vec2 current_value;
        std::suspend_always yield_value(ei::Vec2 value) {
            current_value = value;
            return {};
        }
        AlgorithmGenerator get_return_object() {
            return AlgorithmGenerator{handle_type::from_promise(*this)};
        }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {
            std::exit(1);
        }
    };

    handle_type coro;

    AlgorithmGenerator(handle_type h) : coro(h) {}
    ~AlgorithmGenerator() { coro.destroy(); }

    ei::Vec2 next() {
        coro.resume();
        return coro.promise().current_value;
    }
};

inline AlgorithmGenerator exampleAlgorithm()
{
    for (int i = 0; i < 3; i++)
    {
        co_yield ei::Vec2(static_cast<float>(i), static_cast<float>(i));
    }
}

class VisualAlgorithm {
public:
    VisualAlgorithm() = default;
    VisualAlgorithm(const std::function<AlgorithmGenerator()>& generator_func);

    void draw(sf::Window& window);
    void drawStep(sf::Window& window);
    void step();

private:
    std::function<AlgorithmGenerator()> algorithm_ = nullptr;
};



#endif //VISUALALGORITHM_H
