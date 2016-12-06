#pragma once
#include <SFML/Graphics.hpp>

struct Player
{
	sf::Vector2f position;
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
	Player(const sf::Vector2f &position);
};

