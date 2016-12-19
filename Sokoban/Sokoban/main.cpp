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

void Display(RenderWindow &window, Level &level, std::vector<Box*> &boxes, Player & player, float time)
{
	player.Update(time);
	bool isCollision;
	for (size_t i = 0; i < boxes.size(); ++i)
	{
		isCollision = false;
		if (player.GetRect().intersects(boxes[i]->sprite.getGlobalBounds()) && player.offset.x < 0)
		{
			boxes[i]->sprite.setPosition(boxes[i]->sprite.getPosition().x - 58, boxes[i]->sprite.getPosition().y);

			for (size_t j = 0; j < boxes.size(); ++j)
			{
				if (boxes[i]->sprite.getPosition() == boxes[j]->sprite.getPosition() && (i != j))
				{
					player.position.x -= player.offset.x * time;
					boxes[i]->sprite.setPosition(boxes[i]->sprite.getPosition().x + 58, boxes[i]->sprite.getPosition().y);
					isCollision = true;
				}
			}

			for (size_t j = 0; j < player.obj.size(); ++j)
			{
				
				if ((player.obj[j].rect.intersects((*boxes[i]).sprite.getGlobalBounds())) && (player.obj[j].name == "collision"))
				{
					player.position.x -= player.offset.x * time;
					boxes[i]->sprite.setPosition(boxes[i]->sprite.getPosition().x + 58, boxes[i]->sprite.getPosition().y);
					isCollision = true;
					break;//похоже на костыль
				}

				if ((player.obj[j].rect.intersects((*boxes[i]).sprite.getGlobalBounds())) && (player.obj[j].name == "goal"))
				{

					if (!boxes[i]->isStateFinal)
					{
						boxes[i]->isStateFinal = true;
					}

					isCollision = true;
				}
			}
			if (!isCollision && boxes[i]->isStateFinal)
			{
				boxes[i]->isStateFinal = false;
			}
		}


		if (player.GetRect().intersects(boxes[i]->sprite.getGlobalBounds()) && player.offset.x > 0)
		{
			boxes[i]->sprite.setPosition(boxes[i]->sprite.getPosition().x + 58, boxes[i]->sprite.getPosition().y);
			for (size_t j = 0; j < boxes.size(); ++j)
			{
				if (boxes[i]->sprite.getPosition() == boxes[j]->sprite.getPosition() && (i != j))
				{
					player.position.x -= player.offset.x * time;
					boxes[i]->sprite.setPosition(boxes[i]->sprite.getPosition().x - 58, boxes[i]->sprite.getPosition().y);
					isCollision = true;
				}
			}
			for (size_t j = 0; j < player.obj.size(); ++j)
			{

				if ((player.obj[j].rect.intersects((*boxes[i]).sprite.getGlobalBounds())) && (player.obj[j].name == "collision"))
				{
					player.position.x -= player.offset.x * time;
					boxes[i]->sprite.setPosition(boxes[i]->sprite.getPosition().x - 58, boxes[i]->sprite.getPosition().y);
					isCollision = true;
					break;//похоже на костыль
				}


				if ((player.obj[j].rect.intersects((*boxes[i]).sprite.getGlobalBounds())) && (player.obj[j].name == "goal"))
				{

					if (!boxes[i]->isStateFinal)
					{
						boxes[i]->isStateFinal = true;
					}

					isCollision = true;
				}
			}
			if (!isCollision && boxes[i]->isStateFinal)
			{
				boxes[i]->isStateFinal = false;
			}
		}


		if (player.GetRect().intersects(boxes[i]->sprite.getGlobalBounds()) && player.offset.y > 0)
		{

			boxes[i]->sprite.setPosition(boxes[i]->sprite.getPosition().x, boxes[i]->sprite.getPosition().y + 58);
			for (size_t j = 0; j < boxes.size(); ++j)
			{
				if (boxes[i]->sprite.getPosition() == boxes[j]->sprite.getPosition() && (i != j))
				{
					player.position.y -= player.offset.y * time;
					boxes[i]->sprite.setPosition(boxes[i]->sprite.getPosition().x, boxes[i]->sprite.getPosition().y - 58);
					isCollision = true;
				}
			}
			for (size_t j = 0; j < player.obj.size(); ++j)
			{

				if ((player.obj[j].rect.intersects((*boxes[i]).sprite.getGlobalBounds())) && (player.obj[j].name == "collision"))
				{
					
					player.position.y -= player.offset.y * time;
					boxes[i]->sprite.setPosition(boxes[i]->sprite.getPosition().x, boxes[i]->sprite.getPosition().y - 58);
					isCollision = true;
					break;//похоже на костыль
				}

				if ((player.obj[j].rect.intersects((*boxes[i]).sprite.getGlobalBounds())) && (player.obj[j].name == "goal"))
				{

					if (!boxes[i]->isStateFinal)
					{
						boxes[i]->isStateFinal = true;
					}

					isCollision = true;
				}

			}
			if (!isCollision && boxes[i]->isStateFinal)
			{
				boxes[i]->isStateFinal = false;
			}
		}


		if (player.GetRect().intersects(boxes[i]->sprite.getGlobalBounds()) && player.offset.y < 0)
		{
			boxes[i]->sprite.setPosition(boxes[i]->sprite.getPosition().x, boxes[i]->sprite.getPosition().y - 58);
			for (size_t j = 0; j < boxes.size(); ++j)
			{
				if (boxes[i]->sprite.getPosition() == boxes[j]->sprite.getPosition() && (i != j))
				{

					player.position.y -= player.offset.y * time;
					boxes[i]->sprite.setPosition(boxes[i]->sprite.getPosition().x, boxes[i]->sprite.getPosition().y + 58);
					isCollision = true;
				}
			}
			for (size_t j = 0; j < player.obj.size(); ++j)
			{

				if ((player.obj[j].rect.intersects((*boxes[i]).sprite.getGlobalBounds())) && (player.obj[j].name == "collision"))
				{

					isCollision = true;
					player.position.y -= player.offset.y * time;
					boxes[i]->sprite.setPosition(boxes[i]->sprite.getPosition().x, boxes[i]->sprite.getPosition().y + 58);
					break;//похоже на костыль
				}


				if ((player.obj[j].rect.intersects((*boxes[i]).sprite.getGlobalBounds())) && (player.obj[j].name == "goal"))
				{

					if (!boxes[i]->isStateFinal)
					{
						boxes[i]->isStateFinal = true;
					}

					isCollision = true;
				}
			}

			if (!isCollision && boxes[i]->isStateFinal)
			{
				boxes[i]->isStateFinal = false;
			}
		}
		auto positionSprite = (boxes[i]->isStateFinal) ? Vector2f(0, 116) : Vector2f(0, 58);

		boxes[i]->sprite.setTextureRect(IntRect(static_cast<int>(positionSprite.x), static_cast<int>(positionSprite.y), 58, 58));

	}
	player.sprite.setPosition(player.position);
	window.clear(Color::White);
	level.Draw(window);
	
	size_t k = 0;
	for (auto box : boxes)
	{
		window.draw(box->sprite);
		if (box->isStateFinal)
		{
			++k;
		}
	}
	if (k == boxes.size())
	{
		cout << "win"<< endl;
	}
	window.draw(player.sprite);
	window.display();
}

void EnterGameLoop(RenderWindow &window, Level &level, std::vector<Box*> &boxes, Player & player, Clock & clock)
{
	while (window.isOpen())
	{

		float time = clock.getElapsedTime().asMicroseconds(); //дать прошедшее время в микросекундах
		clock.restart(); //перезагружает время
		time = time / 800; //скорость игры

		HandleEvents(window);
		Display(window, level, boxes, player, time);

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
	player.InitPlayer(level);

	EnterGameLoop(window, level, boxes, player, clock);
	return EXIT_SUCCESS;
}



