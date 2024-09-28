//
// Created by RINI on 25/09/2024.
//

#include "VisualAlgorithm.h"

VisualAlgorithm::VisualAlgorithm(const std::function<AlgorithmGenerator()>& generator_func)
{
    algorithm_ = generator_func;
}

void VisualAlgorithm::draw(sf::Window& window)
{

}

void VisualAlgorithm::drawStep(sf::Window& window)
{

}

void VisualAlgorithm::step()
{

}