#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <sstream> // For displaying equations
#include <functional>
#include <array>

// Compile: g++ -c src/main.cpp -IC:\SFML-3.0.0\include -DSFML_STATIC (make all)
/*
Link progra m: g++ main.o -o main -LC:\SFML-3.0.0\lib -lsfml-graphics-s
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

struct EquationParams {
    std::array<float, 12> coeffs; // up to 12 coefficients
};

// type alias for function pointers
using EquationFunction = std::function<std::pair<float, float>(float, float, float, const EquationParams&)>;
// function to generate new equation parameters
auto generateNewEquationParams = []() {
    EquationParams params;
    for (float& coeff : params.coeffs) {
        coeff = randomFloat(-2.0f, 2.0f);
    }
    return params;
};

// Define equation functions
std::pair<float, float> equation1(float x, float y, float t, const EquationParams& p) {
    return {
        p.coeffs[0] * std::sin(p.coeffs[1] * y + p.coeffs[2] * t) + p.coeffs[3] * std::cos(p.coeffs[4] * x - p.coeffs[5] * t),
        p.coeffs[6] * std::cos(p.coeffs[7] * x - p.coeffs[8] * t) + p.coeffs[9] * std::sin(p.coeffs[10] * y + p.coeffs[11] * t)
    };
}

std::pair<float, float> equation2(float x, float y, float t, const EquationParams& p) {
    return {
        1 - p.coeffs[0] * x * x + y,
        p.coeffs[1] * x
    };
}

int main() {
    const int windowWidth = 1200;
    const int windowHeight = 1200;
    const int trailLength = 15; // Length of the trail ************************ // 200
    const float scale = 200.0f; // Scaling factor // 200.0f

    // SFML window setup
    sf::RenderWindow window(
      sf::VideoMode({windowWidth, windowHeight}),
      "Chaos Equations"
    );
    window.setFramerateLimit(60);

    // Time-related variables
    sf::Clock clock;
    float t = 0.0f; // Initial time
    float speed = 0.00000000000000005f; // Speed of animation ******************************

    // Random seed
    srand(static_cast<unsigned>(time(0)));

    // Points and their initialization
    std::vector<Point> points;
    int pointsToSpawn = 1; // Initial number of points to spawn

    // Load font
    sf::Font font;
    if (!font.openFromFile("assets/tuffy.ttf")) { return -1; }

    // store equation functions in an array
    std::array<EquationFunction, 2> equations = {equation1, equation2};
    EquationParams params = generateNewEquationParams();
    EquationFunction currentEquation = equations[0];
    int currentEquationIndex = 0;

    // Display equation
    sf::Text equationText(font);

    // Main loop
    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        t += deltaTime.asSeconds() * speed;

        // Handle events
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            // switch to a random eq
            currentEquationIndex = rand() % equations.size();
            currentEquation = equations[currentEquationIndex];
            // New parameters for the new equation
            params = generateNewEquationParams();
            t = 0; // reset time for new equation
            points.clear();
            pointsToSpawn = 1;
        }

        // Gradually spawn more points
        if (points.size() < 1000 && t > points.size() / 10.0f) {
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

            // use current equation parameters
            auto [newX, newY] = currentEquation(x, y, t, params); // structured bindings

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
        
        // rendering trails with vertex arrays for more efficiency
        for (const auto& point : points) {
            sf::VertexArray trailArray(sf::PrimitiveType::LineStrip, point.trail.size());
            for (size_t i = 0; i < point.trail.size(); ++i) {
                trailArray[i].position = point.trail[i];
                int maxAlpha = 150;
                float alpha = maxAlpha * std::pow(0.9f, i);
                trailArray[i].color = sf::Color(point.color.r, point.color.g, point.color.b, static_cast<int>(std::max(0.0f, alpha))); // Ensure alpha is not negative            
            }
            window.draw(trailArray);

            sf::CircleShape circle(2.0f);
            circle.setPosition(point.position);
            circle.setFillColor(point.color); // Main points in their color
            window.draw(circle);
        }

        // Display the current equation
        std::stringstream equationTextStream;
        equationTextStream << "Equation: " << currentEquationIndex + 1 << "\n"; // Display the current equation index
        for (size_t i = 0; i < params.coeffs.size(); ++i) {
            equationTextStream << "c[" << i << "] = " << params.coeffs[i] << "\n";
        }
        equationText.setString(equationTextStream.str());
        window.draw(equationText);

        window.display();
    }

    return 0;
}
