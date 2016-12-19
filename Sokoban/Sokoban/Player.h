#pragma once
#include <SFML/Graphics.hpp>
#include "level.h"

enum struct Direction
{
	LEFT = -1,
	STAY = 0,
	RIGHT = 1,
	DOWN = 2,
	UP = 3
};


struct Player
{
	sf::Vector2f position;
	sf::Vector2f offset;
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
	float currentFrame;
	Direction direction;
	std::vector<Object> obj;
	Player(const sf::Vector2f &position);
	void GetDirection();
	void Update(float time);
	void InitPlayer(Level &level);
	sf::FloatRect GetRect();
	void CheckCollision(const float &dx, const float &dy);
};

