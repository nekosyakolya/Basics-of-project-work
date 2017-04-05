#include "stdafx.h"
#include <SFML/Audio.hpp>

#include "level.h"

#include "Box.h"
#include "Player.h"

#include "const.h"

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


void ChangePosition(Box &box, Player & player, const sf::Vector2f &offset)
{
	player.position.x -= player.offset.x;
	player.position.y -= player.offset.y;
	player.direction = Direction::STAY;
	box.sprite.setPosition(box.sprite.getPosition().x + (-offset.x), box.sprite.getPosition().y + (-offset.y));
}

// TODO: pass reference to Box instead of boxNo
bool hasCollisionWithBox(std::vector<Box*> &boxes, Player & player, const size_t &boxNo, const sf::Vector2f &offset)
{
	bool isCollision = false;
	boxes[boxNo]->sprite.setPosition(boxes[boxNo]->sprite.getPosition().x + offset.x, boxes[boxNo]->sprite.getPosition().y + offset.y);
	for (size_t j = 0; j < boxes.size(); ++j)
	{
		auto &box1 = boxes[boxNo];
		auto &box2 = boxes[j];
		if (boxes[boxNo]->sprite.getPosition() == boxes[j]->sprite.getPosition() && (&box1 != &box2))
		{
			ChangePosition(*(boxes[boxNo]), player, offset);
			isCollision = true;
		}
	}
	return isCollision;
}



// TODO: pass reference to Box instead of boxNo
void CheckObjects(std::vector<Box*> &boxes, Player & player, const size_t &boxNo, const sf::Vector2f &offset)
{
	bool isCollision = hasCollisionWithBox(boxes, player, boxNo, offset);

	for (size_t j = 0; j < player.obj.size(); ++j)
	{
		auto &solidObject = player.obj[j];

		if ((solidObject.rect.intersects((*boxes[boxNo]).sprite.getGlobalBounds())) && (solidObject.name == "collision"))
		{
			ChangePosition(*(boxes[boxNo]), player, offset);
			isCollision = true;
			break;
		}

		if ((solidObject.rect.intersects((*boxes[boxNo]).sprite.getGlobalBounds())) && (solidObject.name == "goal"))
		{

			if (!boxes[boxNo]->isStateFinal)
			{
				boxes[boxNo]->isStateFinal = true;
			}

			isCollision = true;
		}
	}
	if (!isCollision && boxes[boxNo]->isStateFinal)
	{
		boxes[boxNo]->isStateFinal = false;
	}
}

void CheckCollision(std::vector<Box*> &boxes, Player & player, float time)
{
	sf::Vector2f offset;
	for (size_t i = 0; i < boxes.size(); ++i)
	{
		if (player.GetRect().intersects(boxes[i]->sprite.getGlobalBounds()))
		{
			player.offset.x *= time;
			player.offset.y *= time;
			if (player.offset.x < 0)
			{
				offset.x = -SIZE_TILE;
				offset.y = 0;
			}
			if (player.offset.x > 0)
			{
				offset.x = SIZE_TILE;
				offset.y = 0;
			}
			if (player.offset.y > 0)
			{

				offset.x = 0;
				offset.y = SIZE_TILE;
			}

			if (player.offset.y < 0)
			{
				offset.x = 0;
				offset.y = -SIZE_TILE;
			}
			CheckObjects(boxes, player, i, offset);
		}


		auto positionSprite = (boxes[i]->isStateFinal) ? Vector2f(0, 116) : Vector2f(0, 58);

		boxes[i]->sprite.setTextureRect(IntRect(static_cast<int>(positionSprite.x), static_cast<int>(positionSprite.y), 58, 58));

	}
}

void DrawAndCheckBoxes(RenderWindow &window, std::vector<Box*> &boxes)
{
	size_t finishedBoxesCount = 0;
	for (auto box : boxes)
	{
		window.draw(box->sprite);
		if (box->isStateFinal)
		{
			++finishedBoxesCount;
		}
	}
	if (finishedBoxesCount == boxes.size())
	{
		cout << "win" << endl;
	}
}

void Display(RenderWindow &window, Level &level, std::vector<Box*> &boxes, Player & player, float time)
{
	player.Update(time);
	
	CheckCollision(boxes, player, time);

	player.sprite.setPosition(player.position);

	window.clear(Color::White);
	level.Draw(window);
	

	DrawAndCheckBoxes(window, boxes);

	window.draw(player.sprite);
	window.display();
}

void EnterGameLoop(RenderWindow &window, Level &level, std::vector<Box*> &boxes, Player & player, Clock & clock)
{
	while (window.isOpen())
	{

		float time = static_cast<float>(clock.getElapsedTime().asMicroseconds()); //дать прошедшее время в микросекундах
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

