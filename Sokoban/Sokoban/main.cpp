#include "stdafx.h"
#include <SFML/Audio.hpp>
#include <string>
#include <sstream>

#include "level.h"

#include "Box.h"
#include "Player.h"

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


/*


void EnterGameLoop(RenderWindow &window, Level &level)
{
	while (window.isOpen())
	{
		HandleEvents(window);
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{ 
			herosprite.move(-0.1, 0); 
			herosprite.setTextureRect(IntRect(0, 66, 96, 56)); 
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) { herosprite.move(0.1, 0); herosprite.setTextureRect(IntRect(0, 132, 96, 56)); } //координата Y, на которой герой изображен идущем вправо равна 96+96=192
		if (Keyboard::isKeyPressed(Keyboard::Up)) { herosprite.move(0, -0.1); herosprite.setTextureRect(IntRect(0, 188, 96, 66)); } //координата Y на которой герой изображен идущим вверх равна 288
		if (Keyboard::isKeyPressed(Keyboard::Down)) { herosprite.move(0, 0.1); herosprite.setTextureRect(IntRect(0, 0, 96, 66)); }
	}
}
*/


void Display(RenderWindow &window, Level &level, std::vector<Box*> &boxes, Player & player)
{
	window.clear(Color::White);
	level.Draw(window);
	for (auto box : boxes)
	{
	window.draw(box->sprite);
	}
	window.draw(player.sprite);
	window.display();
}

void EnterGameLoop(RenderWindow &window, Level &level, std::vector<Box*> &boxes, Player & player)
{
	while (window.isOpen())
	{
		HandleEvents(window);
		
		Display(window, level, boxes, player);

	}
}



int main()
{
	Level level;
	if (!level.LoadFromFile("recources/map.tmx"))
	{
		return EXIT_FAILURE;
	}

	ContextSettings settings;
	settings.antialiasingLevel = 8;

	RenderWindow window(VideoMode(455, 342), "Sokobanchik", Style::Close, settings);

	Vector2f windowCenter = Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

	std::vector<Object> boxesStartPos = level.GetObjects("box");
	std::vector<Box*> boxes;
	bool isFinal;
	for (auto currBox : boxesStartPos)
	{
		isFinal = (currBox.type == "boxFinal") ? true : false;
		boxes.push_back(new Box(Vector2f(currBox.rect.left, currBox.rect.top), isFinal));
		
	}

	Object hero = level.GetObject("player");
	Player player(Vector2f(hero.rect.left, hero.rect.top));

	EnterGameLoop(window, level, boxes, player);
	return EXIT_SUCCESS;
}



