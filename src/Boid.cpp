#include "Boid.hpp"
#include <random>
#include <iostream>


Boid::Boid()
{
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_real_distribution<float> velDist(-3.f, 3.f);

	const int windowWidth = 800;
	const int windowHeight = 600;

	this->m_position = sf::Vector2f(static_cast<float>(std::rand() % windowWidth), static_cast<float>(std::rand() % windowHeight));

	this->m_velocity = sf::Vector2f(velDist(eng), velDist(eng));

	circle.setPosition(m_position);
	circle.setRadius(5.0f);
	circle.setOrigin(circle.getRadius(), circle.getRadius());
	circle.setFillColor(sf::Color::White);

	this->maxForce = 0.2f;
	this->maxSpeed = 4.0f;
}

void Boid::edges()
{
	if (this->m_position.x > 800.0f)
	{
		this->m_position.x = 0.0f;
	}

	else if (this->m_position.x < 0.0f)
	{
		this->m_position.x = 800.0f;
	}

	if (this->m_position.y > 600.0f)
	{
		this->m_position.y = 0.0f;
	}

	else if (this->m_position.y < 0.0f)
	{
		this->m_position.y = 600.0f;
	}

}

float Boid::distance(sf::Vector2f point1, sf::Vector2f point2)
{
	float dx = point2.x - point1.x;
	float dy = point2.y - point1.y;
	return std::sqrt(dx * dx + dy * dy);
}

void Boid::update()
{
	this->m_position += this->m_velocity;
	this->m_velocity += this->m_acceleration;
	this->m_velocity = limit(this->m_velocity, this->maxSpeed);
	circle.setPosition(this->m_position);
	this->m_acceleration = sf::Vector2f(0.0f, 0.0f);
}

sf::Vector2f Boid::align(std::vector<Boid*>& flock)
{
	float perceptionRadius = 50.0f;
	sf::Vector2f desired;
	int total = 0;

	for(auto& boid: flock)
	{
		float d = distance(boid->circle.getPosition(), this->m_position);
		
		if(boid != this && d < perceptionRadius)
		{
			desired += boid->m_velocity;
			total += 1;
		}
	}

	if(total > 0)
	{
		desired.x /= total;
		desired.y /= total;
		desired = setMag(desired, this->maxSpeed);
		desired -= this->m_velocity;
		desired = limit(desired, this->maxForce);
	}
	return desired;
}

sf::Vector2f Boid::cohesion(std::vector<Boid*>& flock)
{
	float perceptionRadius = 100.0f;
	sf::Vector2f desired;
	int total = 0;

	for (auto& boid : flock)
	{
		float d = distance(boid->circle.getPosition(), this->m_position);

		if (boid != this && d < perceptionRadius)
		{
			desired += boid->m_position;
			total += 1;
		}
	}

	if (total > 0)
	{
		desired.x /= total;
		desired.y /= total;
		desired -= this->m_position;
		desired = setMag(desired, this->maxSpeed);
		desired -= this->m_velocity;
		desired = limit(desired, this->maxForce);
	}
	return desired;
}

sf::Vector2f Boid::seperation(std::vector<Boid*>& flock)
{
	float perceptionRadius = 100.0f;
	sf::Vector2f desired;
	int total = 0;

	for (auto& boid : flock)
	{
		float d = distance(boid->circle.getPosition(), this->m_position);

		if (boid != this && d < perceptionRadius)
		{
			sf::Vector2f diff = this->m_position - boid->circle.getPosition();
			diff /=  d;
			desired += diff;
			total += 1;
		}
	}

	if (total > 0)
	{
		desired.x /= total;
		desired.y /= total;
		desired = setMag(desired, this->maxSpeed);
		desired -= this->m_velocity;
		desired = limit(desired, this->maxForce);
	}
	return desired;
}

sf::Vector2f Boid::limit(const sf::Vector2f& vector, float maxMagnitude) 
{
	sf::Vector2f result = vector;

	float magnitude = std::sqrt(result.x * result.x + result.y * result.y);

	if (magnitude > maxMagnitude) 
	{
		result /= magnitude;
		result *= maxMagnitude; 
	}

	return result;
}

sf::Vector2f Boid::setMag(const sf::Vector2f& vector, float magnitude) 
{
	float currentMagnitude = std::sqrt(vector.x * vector.x + vector.y * vector.y);

	if (currentMagnitude != 0) 
	{
		sf::Vector2f normalizedVector = vector / currentMagnitude;
		return normalizedVector * magnitude;
	}
	else 
	{
		return sf::Vector2f(0.0f, 0.0f); 
	}
}


void Boid::flock(std::vector<Boid*>& flock)
{
	sf::Vector2f alignment = align(flock);
	sf::Vector2f m_cohesion = cohesion(flock);
	sf::Vector2f m_seperation = seperation(flock);
	this->m_acceleration += alignment;
	this->m_acceleration += m_cohesion;
	this->m_acceleration += m_seperation;
}

Boid::~Boid() {}
