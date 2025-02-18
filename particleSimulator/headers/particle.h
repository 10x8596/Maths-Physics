#ifndef SOURCE_HEADERS_PARTICLE_H_
#define SOURCE_HEADERS_PARTICLE_H_

#include "includes.h"

class Particle{
public:
	sf::Vector2f _velocity;
	sf::Vector2f _position;
	sf::Color _color;

	Particle(sf::Vector2f initial_position, sf::Color initial_color);
};


#endif