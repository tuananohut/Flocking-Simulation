#include <SFML/Graphics.hpp>
#include "Boid.hpp"
#include <vector>

std::vector<Boid*> flock;

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 10;
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Setup", sf::Style::Default, settings);
	window.setFramerateLimit(60);

	for(int i = 0; i < 100; i++)
	{
		flock.push_back(new Boid());
	}

	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
			{
				window.close();
			}

		}
		
		window.clear(sf::Color::Black);
	
		for(auto& boid : flock)
		{
			boid->edges();
			boid->flock(flock);
			boid->update();
			window.draw(boid->circle);
		}

		window.display();
	}



	return 0;
}
