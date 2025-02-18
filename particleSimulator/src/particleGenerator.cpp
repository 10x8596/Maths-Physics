#include "../headers/ParticleGenerator.h"
#include "../headers/includes.h"


const float ParticleGenerator::max_velocity = 30;

float sqrt7(float x){
	unsigned int i = *(unsigned int*) &x;
	i += 127<<23;
	i >>= 1;
	return *(float*) &i;
}

float getMagnitude(const sf::Vector2f &vec){
	return sqrt7(vec.x*vec.x+vec.y*vec.y);
}

void normalize(sf::Vector2f &vec){
	vec /= getMagnitude(vec);
}

ParticleGenerator::ParticleGenerator(sf::Vector2f &room_dimensions, int number_of_particles):
	_NUMBER_OF_PARTICLES(number_of_particles),
	_FRICTION(0.001)
	{

	int offset = room_dimensions.x/10;
	std::random_device rd;
	std::mt19937 e2(rd());

	std::uniform_int_distribution<> width_distribution(offset,room_dimensions.x-offset);
	std::uniform_int_distribution<> height_distribution(offset,room_dimensions.y-offset);
	std::uniform_int_distribution<> opacity_distribution(50,250);

	float random_position_x;
	float random_position_y;

	sf::Color color = sf::Color(125,57,125,0);

	Particle* temp;

	for(int i=0;i<_NUMBER_OF_PARTICLES;i++){
		random_position_x = width_distribution(e2);
		random_position_y = height_distribution(e2);
		color.a = opacity_distribution(e2);

		temp = new Particle(sf::Vector2f(random_position_x, random_position_y),color);

		_list_of_particles.push_back(temp);
	}
}

void ParticleGenerator::update(sf::Vector2f &mouse_position, sf::Time dt){

	for(Particle* p:_list_of_particles){

		sf::Vector2f &pos = p->_position;

		sf::Vector2f acceleration = mouse_position-pos;
		normalize(acceleration);

		sf::Vector2f &velocity =p->_velocity;

		velocity += acceleration*(5*dt.asSeconds());

		velocity -= ParticleGenerator::_FRICTION*velocity;

		pos += velocity;



	}
}

const std::list<Particle*>& ParticleGenerator::getParticles(){
	return _list_of_particles;
}