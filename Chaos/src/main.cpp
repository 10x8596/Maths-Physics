#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <sstream> // For displaying equations
#include <functional>
#include <array>
#include <SFML/Graphics/VertexArray.hpp> // Include for VertexArray

// Compile: g++ -c src/main.cpp -IC:\SFML-3.0.0\include -DSFML_STATIC (make all)
/*
Link progra m: g++ main.o -o main -LC:\SFML-3.0.0\lib -lsfml-graphics-s
-lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -mwindows
*/
// execute: ./main.exe

/*
TODO: draw the current equation on screen
*/

// Generate a random floating-point number in a range
float randomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / 
    (static_cast<float>(RAND_MAX) / (max - min));
}

// Point structure
struct Point {
    sf::Vector3f position;
    sf::Color color;
    std::vector<sf::Vector3f> trail; // Trail for points
    sf::Vector3f velocity;
};

struct EquationParams {
    std::array<float, 4> coeffs; // up to 4 coefficients
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
        p.coeffs[0] * std::sin(p.coeffs[1] * y + t) + p.coeffs[2] * std::cos(p.coeffs[3] * x - t),
        p.coeffs[2] * std::sin(p.coeffs[0] * x - t) + p.coeffs[3] * std::cos(p.coeffs[1] * y + t)
    };
}

std::pair<float, float> equation2(float x, float y, float t, const EquationParams& p) {
    return {
        p.coeffs[0] * std::cos(p.coeffs[1] * y + t) + p.coeffs[2] * std::sin(p.coeffs[3] * x - t),
        p.coeffs[2] * std::cos(p.coeffs[0] * x - t) + p.coeffs[3] * std::sin(p.coeffs[1] * y + t)
    };
}

std::pair<float, float> equation3(float x, float y, float t, const EquationParams& p) {
    return {
        p.coeffs[0] * std::sin(p.coeffs[1] * y + t) + p.coeffs[2] * std::cos(p.coeffs[3] * x - t),
        p.coeffs[2] * std::cos(p.coeffs[0] * x - t) + p.coeffs[3] * std::sin(p.coeffs[1] * y + t)
    };
}

std::pair<float, float> equation4(float x, float y, float t, const EquationParams& p) {
    return {
        p.coeffs[0] * std::cos(p.coeffs[1] * y + t) + p.coeffs[2] * std::sin(p.coeffs[3] * x - t),
        p.coeffs[2] * std::sin(p.coeffs[0] * x - t) + p.coeffs[3] * std::cos(p.coeffs[1] * y + t)
    };
}

int main() {
    const unsigned windowWidth = 1200;
    const unsigned windowHeight = 1200;
    const int trailLength = 1.5; // Length of the trail ************************ // 200
    const float scale = 200.0f; // Scaling factor // 200.0f

    // SFML window setup
    sf::RenderWindow window(
      sf::VideoMode({windowWidth, windowHeight}),
      "Chaos Equations",
      sf::Style::Default
    );
    window.setFramerateLimit(165);

    // Time-related variables
    sf::Clock clock;
    float t = 0.0f; // Initial time
    float speed = 5.0E-5F; // Speed of animation ******************************

    // Random seed
    srand(static_cast<unsigned>(time(0)));

    // Points and their initialization
    std::vector<Point> points;
    int pointsToSpawn = 1; // Initial number of points to spawn

    // Load font
    sf::Font font;
    if (!font.openFromFile("assets/tuffy.ttf")) { return -1; }

    // store equation functions in an array
    std::array<EquationFunction, 4> equations = {equation1, equation2, equation3, equation4};
    EquationParams params = generateNewEquationParams();
    EquationFunction currentEquation = equations[0];
    int currentEquationIndex = 0;

    // Display equation
    sf::Text equationText(font);

    // Main loop
    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        t += deltaTime.asSeconds() * speed; // *******************************************

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
        if (points.size() < 1200 && t > points.size() / 10.0f) {
            for (int i = 0; i < pointsToSpawn; ++i) {
                Point newPoint;
                newPoint.position = sf::Vector3f(
                  randomFloat(0, windowWidth),
                  randomFloat(0, windowHeight),
                  randomFloat(-50, 50)       // Initialize z (experiment with the range)
                );
                newPoint.color = sf::Color(
                  rand() % 256,
                  rand() % 256,
                  rand() % 256
                );
                newPoint.velocity = sf::Vector3f(
                  randomFloat(-1, 1),
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
            float z = point.position.z / scale;

            // use current equation parameters
            auto [newX, newY] = currentEquation(x, y, t, params); // structured bindings

            // Store the point's position from the previous frame
            sf::Vector3f prevPosition = point.position;

            point.position.x = newX * scale + windowWidth / 8.0f;
            point.position.y = windowHeight / 8.0f - newY * scale;
            point.position.z = z + (x * y - 8/3 * z) * 0.01f * scale; // Update z (Euler's method)

            // Interpolation for smoother animation (3D)
            /* A value between 0 and 1 that determines how much to interpolate. 
            based on deltaTime and speed. Experiment with the multiplier 
            to adjust the sensitivity of the interpolation. Higher = smoother*/
            float interpolationFactor = deltaTime.asSeconds() * speed * 100; 
            interpolationFactor = std::min(1.0f, interpolationFactor); // clamp to 1

            // interpolatedPosition: calculated as a blend between the previous and current positions
            sf::Vector3f interpolatedPosition = prevPosition + (point.position - prevPosition) * interpolationFactor;

            // Update trail
            if (point.trail.size() > trailLength) {
                point.trail.erase(point.trail.begin());
            }
            point.trail.push_back(point.position);

            // Draw the interpolated position:
            sf::CircleShape circle(2.0f);
            circle.setPosition(sf::Vector2f(interpolatedPosition.x, interpolatedPosition.y)); 
            circle.setFillColor(point.color);
            window.draw(circle);

            // calculate moving average of point's speed to smooth out changes in position
            static float averageSpeed = speed;
            float currentSpeed = std::max(0.1f, std::fabs(newX - x) + std::fabs(newY - y));
            averageSpeed = 0.8f * averageSpeed + 0.2f * currentSpeed; // Adjust 0.9 and 0.1 for smoothing
            speed = averageSpeed;
        }

        // Render points
        window.clear(sf::Color::Black);
        
        // rendering trails with vertex arrays for more efficiency
        for (const auto& point : points) {
            sf::VertexArray trailArray(sf::PrimitiveType::LineStrip, point.trail.size());
            for (size_t i = 0; i < point.trail.size(); ++i) {
                // Project 3D trail points to 2D
                float trailX = point.trail[i].x / scale * 200 + windowWidth / 2;
                float trailY = point.trail[i].y / scale * 200 + windowHeight / 2;
                trailArray[i].position = sf::Vector2f(trailX, trailY);
                int maxAlpha = 150;
                float alpha = maxAlpha * std::pow(0.9f, i);
                trailArray[i].color = sf::Color(point.color.r, point.color.g, point.color.b, static_cast<int>(std::max(0.0f, alpha)));
            }
            window.draw(trailArray);
        }

        // Display the current equation
        std::stringstream equationTextStream; int idx = currentEquationIndex + 1;
        // use switch case to cycle through different equations
        switch(idx) {
            case 1:
                equationTextStream << "Equation: " 
                               << "x: c[0] * sin(c[1] * y + t) + c[2] * cos(c[3] * x - t)" << "\n" 
                               << "y: c[2] * sin(c[0] * x - t) + c[3] * cos(c[1] * y + t)" << "\n";
                break;
            case 2:
                equationTextStream << "Equation: " 
                               << "x: c[0] * cos(c[1] * y + t) + c[2] * sin(c[3] * x - t)" << "\n" 
                               << "y: c[2] * cos(c[0] * x - t) + c[3] * sin(c[1] * y + t)" << "\n";
                break;
            case 3:
                equationTextStream << "Equation: " 
                               << "x: c[0] * sin(c[1] * y + t) + c[2] * cos(c[3] * x - t)" << "\n" 
                               << "y: c[2] * cos(c[0] * x - t) + c[3] * sin(c[1] * y + t)" << "\n";
                break;
            case 4:
                equationTextStream << "Equation: " 
                               << "x: c[0] * cos(c[1] * y + t) + c[2] * sin(c[3] * x - t)" << "\n" 
                               << "y: c[2] * sin(c[0] * x - t) + c[3] * cos(c[1] * y + t)" << "\n";
                break;
            default:
                break;
        }

        for (size_t i = 0; i < params.coeffs.size(); ++i) {
            equationTextStream << "c[" << i << "] = " << params.coeffs[i] << "\n";
        }
        equationText.setString(equationTextStream.str());
        window.draw(equationText);

        window.display();
    }

    return 0;
}
