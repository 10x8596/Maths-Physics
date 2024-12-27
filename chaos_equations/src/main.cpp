#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <iostream>

// Compile: g++ -IC:\SFML-3.0.0\include -c src/main.cpp -o chaos_equations.o
// Link program: g++ -IC:\SFML-3.0.0\lib .\chaos_equations.o -o chaos_equations.exe -lmingw32 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-main -mwindows

// Chaos equations
void computeNextState(float& x, float& y, float t, float dt) {
    // Define equations
    float xPrime = -std::pow(x, 2) + x + y;
    float yPrime = x - std::pow(y, 2) - x * y + y * t - x + y;
    x += xPrime * dt;
    y += yPrime * dt;
}

int main () {

    return 0;
}