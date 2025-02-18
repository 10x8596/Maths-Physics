#ifndef SOURCE_HEADERS_PARTICLEGENERATOR_H_
#define SOURCE_HEADERS_PARTICLEGENERATOR_H_

#include "includes.h"
#include "particle.h"

class ParticleGenerator{
private:

	const int _NUMBER_OF_PARTICLES;
	const float _FRICTION;
public:

	static const float max_velocity;
	std::list<Particle*> _list_of_particles;

	ParticleGenerator(sf::Vector2f &room_dimensions, int number_of_particle);
	void update(sf::Vector2f &mouse_position, sf::Time dt);

	const std::list<Particle*>& getParticles();

};



#endif