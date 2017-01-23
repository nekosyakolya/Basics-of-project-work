

#include "stdafx.h"
#include "level.h"

#include <SFML/Audio.hpp>


void HandleEvents(sf::RenderWindow &window)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed ||
			(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
		{
			window.close();
		}
	}
}


void EnterGameLoop(CGame &game, sf::RenderWindow &window, sf::Clock &clock)
{
	while (window.isOpen())
	{
		float time = static_cast<float>(clock.getElapsedTime().asMicroseconds());
		clock.restart();
		time = time / 1000;
		HandleEvents(window);

		game.Update(time);
		game.Display(window);
	}

}


int main()
{
	sf::ContextSettings settings;
	sf::RenderWindow window(sf::VideoMode(680, 500), "Game", sf::Style::Close, settings);

	CGame game;

	game.Initialisation();

	sf::Clock clock;

	EnterGameLoop(game, window, clock);
	return 0;
}

