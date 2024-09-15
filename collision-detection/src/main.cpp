#include <SFML/Graphics.hpp>
#include <cmath>

int main()
{
    // Create a window with SFML
    sf::RenderWindow window(sf::VideoMode(800, 600), "Moving Sphere");

    // Set up the sphere (circle shape in 2D)
    sf::CircleShape sphere(25); // Radius of 25 pixels
    sphere.setFillColor(sf::Color::Green);

    // Define the parameters for circular motion
    float radius = 150.0f;                    // Radius of the circular path
    sf::Vector2f center(400, 300);            // Center of the circular path (window center)
    float angle = 0.0f;                       // Starting angle in radians
    float angularSpeed = 0.005f;               // Speed of rotation (radians per frame)

    // Main loop
    while (window.isOpen())
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Update the position of the sphere in a circular motion
        float x = center.x + radius * std::cos(angle);
        float y = center.y + radius * std::sin(angle);
        sphere.setPosition(x, y);

        // Increment the angle to keep the sphere moving
        angle += angularSpeed;

        // Clear the window with a black color
        window.clear(sf::Color::Black);

        // Draw the sphere
        window.draw(sphere);

        // Display the contents of the window
        window.display();
    }

    return 0;
}
