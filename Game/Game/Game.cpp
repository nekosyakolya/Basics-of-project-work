#include "stdafx.h"

#include <SFML/Audio.hpp>
#include <string>
#include <sstream>
#include "player.h"

#include "level.h"

using namespace sf;
using namespace std;

void HandleEvents(RenderWindow &window)
{
	Event event;
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed ||
			(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
		{
			window.close();
		}
	}
}

void UpdateView(float x, float y, View &view)
{
	view.setCenter(x + 20, y - 213);
}

void Display(RenderWindow &window, CPlayer &player, View &view, Level & level, float time)
{
	window.setView(view);
	window.clear(Color::White);
	level.Draw(window);

	player.Update(time);

	UpdateView(320, player.GetPosition().y, view);
	window.draw(player.GetPlayer());
	window.display();
}

void EnterGameLoop(RenderWindow &window, CPlayer &player, View &view, Level & level, Clock & clock)
{
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;
		HandleEvents(window);
		Display(window, player, view, level, time);
	}
}


int main()
{
	Level level;
	if (!level.LoadFromFile("recources/map.tmx"))
	{
		return EXIT_FAILURE;
	}

	Clock clock;
	ContextSettings settings;
	sf::View view;
	settings.antialiasingLevel = 8;
	RenderWindow window(VideoMode(680, 500), "Game", Style::Close, settings);
	Vector2f windowCenter = Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
	view.reset(sf::FloatRect(0, 0, 680, 500));

	Object hero = level.GetObject("player");
	CPlayer player(Vector2f(hero.rect.left, hero.rect.top));
	player.InitPlayer(level);

	EnterGameLoop(window, player, view, level, clock);

	return EXIT_SUCCESS;
}
