#ifndef SOURCE_HEADERS_GAME_H_
#define SOURCE_HEADERS_GAME_H_

#include "ParticleGenerator.h"
#include "includes.h"

class Game{
private:

	void _processInput();
	void _update(sf::Time dt);
	void _render();

	sf::RenderWindow* _window;
	sf::VertexArray* _particles;
	ParticleGenerator* _generator;
	sf::Vector2f _mouse_position;

public:
	sf::Vector2f room_dimensions;
	int number_of_particles;

	void run();
	Game();
	~Game();
};



#endif