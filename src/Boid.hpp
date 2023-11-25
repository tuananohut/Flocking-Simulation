#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Boid
{
public:
	Boid();
	~Boid();

	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	sf::Vector2f m_acceleration;
	float maxForce = 0.1f;
	float maxSpeed = 4.0f;

	sf::Vector2f align(std::vector<Boid*>& flock);
	sf::Vector2f cohesion(std::vector<Boid*>& flock);
	sf::Vector2f seperation(std::vector<Boid*>& flock);

	void update();
	void flock(std::vector<Boid*>& flock);
	void edges();

	float distance(sf::Vector2f point1, sf::Vector2f point2);
	static sf::Vector2f limit(const sf::Vector2f& vector, float maxMagnitude);
	sf::Vector2f setMag(const sf::Vector2f& vector, float magnitude);

	sf::CircleShape circle;
};