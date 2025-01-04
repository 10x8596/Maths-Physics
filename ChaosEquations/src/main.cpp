#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <ctime>


// Compile: g++ -c src/main.cpp -IC:\SFML-3.0.0\include -DSFML_STATIC (make all)
/* Link program: g++ main.o -o main -LC:\SFML-3.0.0\lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -mwindows */
// execute: ./main.exe

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

/* These are plots of random recursive equations, which often produce chaos,
and results in beautiful patterns. For every time t, a point (x,y) 
is initialized to (t,t). The equation is applied to the point many times, 
and each iteration is drawn in a unique color.*/ 
int main(int argc, char *argv[]) {

    const int windowWidth = 1200;
    const int windowHeight = 1200;
    // Number of points to draw per equation
    const int maxIterations = 50000; 
    // scaling for visualization
    const float scale = 200.0f;
    // Points plotted per second
    const float updateSpeed = 1.0f; // higher = faster

    // Init SFML window
    sf::RenderWindow window(sf::VideoMode({windowWidth, windowHeight}), "Chaos Visualization");
    window.setFramerateLimit(1);

    // Clock to measure elapsed time
    sf::Clock clock; 

    // set the random seed
    srand(static_cast<unsigned>(time(0)));

    // create vertices for plotting
    std::vector<sf::Vertex> points(maxIterations);
    std::vector<sf::Vector2f> pointPositions(maxIterations);
    int currentPointIndex = 0;

    // Create the first set of points at t = 0
     for (int i = 0; i < maxIterations; i++) {
        points[i].color = sf::Color::White;
        pointPositions[i] = sf::Vector2f(windowWidth/2, windowHeight/2);
     }

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();

        // Process events
        while (const std::optional event = window.pollEvent()) {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Generate random coefficients for the equation
        float a = randomFloat(-2.0f, 2.0f);
        float b = randomFloat(-2.0f, 2.0f);
        float c = randomFloat(-2.0f, 2.0f);
        float d = randomFloat(-2.0f, 2.0f);
        float x = 0.0f, y = 0.0f;

        // Plot the points
        for (int i = 0; i < maxIterations; ++i) {
            // Apply the recursive equations
            float newX = a * sin(b * y) + c * cos(d * x);
            float newY = c * sin(a * x) + d * cos(b * y);
            x = newX; y = newY;

            // Map to screen coords
            float screenX = windowWidth / 2 + x * scale;
            float screenY = windowHeight / 2 - y * scale;

            // Gradually move the point towards its new position based on update speed
            pointPositions[i].x += (screenX - pointPositions[i].x) * updateSpeed;
            pointPositions[i].y += (screenY - pointPositions[i].y) * updateSpeed;

            points[i].position = pointPositions[i];

            // Assign a color based on the iteration
            points[i].color = sf::Color(
                static_cast<std::uint8_t>((i * 7) % 255),
                static_cast<std::uint8_t>((i * 13) % 255),
                static_cast<std::uint8_t>((i * 29) % 255)
            );
        }

        // Clear and draw points
        window.clear(sf::Color::Black);
        window.draw(&points[0], points.size(), sf::PrimitiveType::Points);
        window.display();
    }

    // return 0;
}