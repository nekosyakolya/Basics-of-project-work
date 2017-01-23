// newGame.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "level.h"

#include <SFML/Audio.hpp>



void Display(CGame &game, sf::RenderWindow &window)
{
	window.setView(game.view);

	window.clear(sf::Color::White);
	game.GetLevel().Draw(window);

	window.draw(game.GetPlayer().GetHero());
	window.display();
}

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
		time = time / 800;



		HandleEvents(window);

		game.UpdatePlayer(time);
		game.view.setCenter(340, game.GetPlayer().GetPosition().y - 213);
		Display(game, window);
	}

}


int main()
{
	sf::ContextSettings settings;
	sf::RenderWindow window(sf::VideoMode(680, 500), "Game", sf::Style::Close, settings);

	CGame game;

	game.view.reset(sf::FloatRect(0, 0, 680, 500));

	game.Initialisation();

	sf::Clock clock;

	EnterGameLoop(game, window, clock);
	return 0;
}

