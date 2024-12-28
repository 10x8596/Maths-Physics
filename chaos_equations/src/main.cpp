#include <SDL2/SDL.h>
#include <cmath>
#include <vector>
#include <iostream>

// Chaos equations
void computeNextState(float& x, float& y, float t, float dt) {
    // Define equations
    float xPrime = -std::pow(x, 2) + x + y;
    float yPrime = x - std::pow(y, 2) - x * y + y * t - x + y;
    x += xPrime * dt;
    y += yPrime * dt;
}

int main () {

    // SFML setup
    sf::RenderWindow window(sf::VideoMode(800, 800), "Chaos Equations");
    window.setFramerateLimit(60);

    // Initial conditions
    float x = 0.1f, y = 0.1f, t = 0.0f, dt = 0.01f;
    std::vector<sf::Vector2f> points;

    // Simulation loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Compute next state
        computeNextState(x, y, t, dt);
        t += dt;

        // Transform coordinates for screen display
        float screenX = 400 + x * 100;
        float screenY = 400 - y * 100;
        points.emplace_back(screenX, screenY);

        // Render
        window.clear(sf::Color::Black);

        // Draw all points
        for (const auto& point : points) {
            sf::CircleShape shape(1);
            shape.setPosition(point.x, point.y);
            shape.setFillColor(sf::Color::White);
            window.draw(shape);
        }

        window.display();
    }

    return 0;
}