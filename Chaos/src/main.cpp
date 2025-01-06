#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <ctime>


// Compile: g++ -c src/main.cpp -IC:\SFML-3.0.0\include -DSFML_STATIC (make all)
/*
Link program: g++ main.o -o main -LC:\SFML-3.0.0\lib -lsfml-graphics-s
-lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -mwindows
*/
// execute: ./main.exe

/*
These are plots of random recursive equations, which often produce chaos,
and results in beautiful patterns. The equations are dynamic systems so they
change over time. For every time t, a point (x,y) is initialized to (t,t). It
starts at a low value and slowly
increases as the animation progresses. at every time step, we start with a point
whose coordinates are initialised to t (x=t, y=t). Then we apply an equation to
update the point such as x' = yt + x - t and y' = x^2 - y^2 - t^2. These
equations can be random. Once we have the point we draw it to the screen with a
unique color and repeat. These points will also have a trail. Essentially once
we have an equation. Let's say x' = yt + x - t and y' = x^2 - y^2 - t^2, we draw
the first point in green, then apply the same equation again to update the point
and draw it in blue and we repeat this process. As time changes, those points
changes as well. We'll also dynamically change the speed so it speeds up
when nothing interesting is happening.
*/

/*
TODO: draw the current equation on screen
TODO: implement a key press feature to generate the next random eq
*/

// Generate a random floating-point number in a range
float randomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / 
    (static_cast<float>(RAND_MAX) / (max - min));
}

// Point structure
struct Point {
    sf::Vector2f position;
    sf::Color color;
    std::vector<sf::Vector2f> trail; // Trail for points
    sf::Vector2f velocity;
};

int main() {
    const int windowWidth = 1200;
    const int windowHeight = 1200;
    const int trailLength = 200; // Length of the trail ************************
    const float scale = 200.0f; // Scaling factor

    // SFML window setup
    sf::RenderWindow window(
      sf::VideoMode({windowWidth, windowHeight}),
      "Chaos Equations"
    );
    window.setFramerateLimit(60);

    // Time-related variables
    sf::Clock clock;
    float t = 0.0f; // Initial time
    float speed = 0.00005f; // Speed of animation ******************************

    // Random seed
    srand(static_cast<unsigned>(time(0)));

    // Dynamic system coefficients
    float a = randomFloat(-2.0f, 2.0f);
    float b = randomFloat(-2.0f, 2.0f);
    float c = randomFloat(-2.0f, 2.0f);
    float d = randomFloat(-2.0f, 2.0f);

    // Points and their initialization
    std::vector<Point> points;
    int pointsToSpawn = 1; // Initial number of points to spawn

    // Main loop
    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        t += deltaTime.asSeconds() * speed;

        // Handle events
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
        }

        // Gradually spawn more points
        if (points.size() < 5000 && t > points.size() / 10.0f) {
            for (int i = 0; i < pointsToSpawn; ++i) {
                Point newPoint;
                newPoint.position = sf::Vector2f(
                  randomFloat(0, windowWidth),
                  randomFloat(0, windowHeight)
                );
                newPoint.color = sf::Color(
                  rand() % 256,
                  rand() % 256,
                  rand() % 256
                );
                newPoint.velocity = sf::Vector2f(
                  randomFloat(-1, 1),
                  randomFloat(-1, 1)
                );
                points.push_back(newPoint);
            }
            pointsToSpawn += 1; // Increase spawn rate
        }

        // Update point positions using dynamic equations
        for (auto& point : points) {
            float x = point.position.x / scale - windowWidth / (2 * scale);
            float y = point.position.y / scale - windowHeight / (2 * scale);

            float newX = a * std::sin(b * y + t) + c * std::cos(d * x - t);
            float newY = c * std::sin(a * x - t) + d * std::cos(b * y + t);

            point.position.x = windowWidth / 2 + newX * scale;
            point.position.y = windowHeight / 2 - newY * scale;

            // Update trail
            if (point.trail.size() > trailLength) {
                point.trail.erase(point.trail.begin());
            }
            point.trail.push_back(point.position);

            // Adjust speed based on changes
            speed = std::max(0.1f, std::fabs(newX - x) + std::fabs(newY - y));
        }

        // Render points
        window.clear(sf::Color::Black);
        sf::VertexArray pointArray(sf::PrimitiveType::Points);
        for (const auto& point : points) {
            // Draw trail
            for (const auto& trailPos : point.trail) {
                sf::CircleShape trailPoint(1.0f);
                trailPoint.setPosition(trailPos);
                trailPoint.setFillColor(sf::Color(
                  point.color.r, point.color.g, point.color.b, 50
                ));
                window.draw(trailPoint);
            }
            // Draw main point
            sf::CircleShape circle(2.0f);
            circle.setPosition(point.position);
            circle.setFillColor(sf::Color::Black);
            window.draw(circle);
        }

        window.display();
    }

    return 0;
}
