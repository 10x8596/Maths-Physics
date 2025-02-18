#include "../headers/Game.h"

Game::~Game(){
    delete _window;
    delete _generator;
    delete _particles;
}

Game::Game(){
	room_dimensions = sf::Vector2f(1280,720);
	number_of_particles = 170000;

    _window = new sf::RenderWindow(
        sf::VideoMode({static_cast<unsigned int>(std::round(room_dimensions.x)), static_cast<unsigned int>(std::round(room_dimensions.y))}),
        "Chaos Equations",
        sf::Style::Default
    );

	_generator = new ParticleGenerator(room_dimensions,number_of_particles);
	_particles = new sf::VertexArray(sf::PrimitiveType::Points, number_of_particles);

	_mouse_position = sf::Vector2f(room_dimensions.x/2,room_dimensions.y/2);
}


void Game::_processInput(){

    while (std::optional<sf::Event> event = _window->pollEvent()) {
        if (event->is<sf::Event::Closed>()) _window->close();
        if (event->is<sf::Event::MouseMoved>()) _mouse_position = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*_window));
        if (event->is<sf::Event::MouseLeft>()) _mouse_position = sf::Vector2f(room_dimensions.x/2,room_dimensions.y/2);
    }

}

void Game::_update(sf::Time dt){
	const std::list<Particle*> &p = _generator->getParticles();

	_generator->update(_mouse_position,dt);

	int i=0;
	for(Particle* temp:p){
		(*_particles)[i].position = temp->_position;
		(*_particles)[i].color = temp->_color;
		i++;
	}

}

void Game::_render(){
	_window->clear(sf::Color::Black);
	_window->draw(*_particles, sf::BlendAdd);
	_window->display();
}

void Game::run(){
	sf::Clock game_clock;
	sf::Time time_per_frame = sf::seconds(1.f/60.f);
	sf::Time time_since_last_update = sf::Time::Zero;
	sf::Time time_sleep;

	while(_window->isOpen()){
		time_sleep = sf::Time::Zero;
		_processInput();
		time_since_last_update += game_clock.restart();


		if(time_since_last_update<time_per_frame) time_sleep = time_per_frame - time_since_last_update;

		while(time_since_last_update>time_per_frame){
			time_since_last_update -= time_per_frame;
			_processInput();
			_update(time_per_frame);
		}

		_render();

        sf::sleep(time_sleep);
	}
}