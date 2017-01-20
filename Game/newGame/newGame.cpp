// newGame.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "level.h"


void Display(CGame &game, sf::RenderWindow &window)
{
	window.clear(sf::Color::White);
	game.GetLevel().Draw(window);
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


void EnterGameLoop(CGame &game, sf::RenderWindow &window)
{
	while (window.isOpen())
	{
		HandleEvents(window);
		Display(game, window);
	}

}


int main()
{
	sf::ContextSettings settings;
	sf::RenderWindow window(sf::VideoMode(680, 500), "Game", sf::Style::Close, settings);

	CGame game;
	game.Init();

	EnterGameLoop(game, window);
	return 0;
}

