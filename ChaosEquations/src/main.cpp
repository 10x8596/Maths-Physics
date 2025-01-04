#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <ctime>


// Compile: g++ -c src/main.cpp -IC:\SFML-3.0.0\include -DSFML_STATIC (make all)
/* Link program: g++ main.o -o main -LC:\SFML-3.0.0\lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -mwindows */
// execute: ./main.exe

/* These are plots of random recursive equations, which often produce chaos,
and results in beautiful patterns. For every time t, a point (x,y) 
is initialized to (t,t). The equation is applied to the point many times, 
and each iteration is drawn in a unique color.*/ 

// Random Equation Generator
/* Generate coefficients a,b,c,d for equations of the form:
    x_(n+1) = a * sin(b * y_n) + c * cos(d * x_n)
    y_(n+1) = c * sin(a * x_n) + d * cos(b * y_n)
*/

// Iteratively plot the points, starting with (x_0, y_0) = (t,t)

// Generate a random floating-point number in a range
float randomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (max - min));
}

int main(int argc, char *argv[]) {

    const int windowWidth = 1200;
    const int windowHeight = 1200;
    // Number of points to draw per equation
    const int maxIterations = 2000; 
    // scaling for visualization
    const float scale = 100.0f;
    // Strength of chaotic movement
    const float chaosFactor = 0.1f;
    // Strength of attraction to targets
    const float attractionStrength = 1.0f;
    // How long to display the full equation before scattering
    const float displayTime = 5.0f; 


    // Init SFML window
    sf::RenderWindow window(sf::VideoMode({windowWidth, windowHeight}), "Chaos Visualization");
    window.setFramerateLimit(60);

    // Clock to measure elapsed time
    sf::Clock clock; 

    // set the random seed
    srand(static_cast<unsigned>(time(0)));

    struct Point {
        sf::Vector2f position;
        sf::Vector2f velocity;
        sf::Vector2f target;
        sf::Color color;
    };

    // create vertices for plotting
    std::vector<Point> points(maxIterations);
    bool equationDisplayed = false;
    float timeSinceLastAction = 0.0f;

    // Initialize points with random positions and velocities
     for (auto& point : points) {
        point.position = sf::Vector2f(randomFloat(0, windowWidth), randomFloat(0, windowHeight));
        point.velocity = sf::Vector2f(randomFloat(-1.0f, 1.0f), randomFloat(-1.0f, 1.0f));
        point.target = point.position; // temporary
        point.color = sf::Color::White;
    }

    // Main loop
    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        timeSinceLastAction += deltaTime.asSeconds();

        // Process events
        while (const std::optional event = window.pollEvent()) {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Generate new equation and set targets if necessary
        if (!equationDisplayed || timeSinceLastAction > displayTime) {
            // Generate new equation for coefficients
            float a = randomFloat(-2.0f, 2.0f);
            float b = randomFloat(-2.0f, 2.0f);
            float c = randomFloat(-2.0f, 2.0f);
            float d = randomFloat(-2.0f, 2.0f);

            // Generate targets based on the equation
            float x = 0.0f, y = 0.0f;
            // Plot the points
            for (int i = 0; i < maxIterations; ++i) {
                // Apply the recursive equations
                float newX = a * sin(b * y) + c * cos(d * x);
                float newY = c * sin(a * x) + d * cos(b * y);
                x = newX; y = newY;

                // Store the target screen position
                points[i].target = sf::Vector2f(
                    windowWidth / 2 + newX * scale,
                    windowHeight / 2 - newY * scale
                );
            }

            // Reset animation state for new points
            equationDisplayed = true;
            // Reset timer for the display time
            timeSinceLastAction = 0.0f;
        }

        // Update points
        for (auto& point : points) {
            // Apply chaotic movement
            point.velocity += sf::Vector2f(
                randomFloat(-chaosFactor, chaosFactor),
                randomFloat(-chaosFactor, chaosFactor)
            );
            // Apply attraction to target
            sf::Vector2f attraction = point.target - point.position;
            float distance = std::hypot(attraction.x, attraction.y);
            if (distance > 0.5f) { // Avoid division by zero
                attraction *= attractionStrength / distance;
                point.velocity += attraction;
            }
            // Update position based on velocity
            point.position += point.velocity;

            // Clamp points within the window bounds
            if (point.position.x < 0 || point.position.x > windowWidth) point.velocity.x *= -1;
            if (point.position.y < 0 || point.position.y > windowHeight) point.velocity.y *= -1;
        }
        
        // Scatter points off-screen after display time
        if (equationDisplayed && timeSinceLastAction > displayTime) {
            for (auto& point : points) {
                point.velocity = sf::Vector2f(
                    randomFloat(-3.0f, 3.0f),
                    randomFloat(-3.0f, 3.0f)
                );
            }
            equationDisplayed = false;
        }

        // Render
        window.clear(sf::Color::Black);
        sf::VertexArray pointArray(sf::PrimitiveType::Points, points.size());
        for (size_t i = 0; i < points.size(); i++) {
            pointArray[i].position = points[i].position;
            pointArray[i].color = points[i].color;
        }

        window.draw(pointArray);
        window.display();
    }

    // return 0;
}