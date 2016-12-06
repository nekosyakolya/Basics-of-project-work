#pragma once
#include <SFML/Graphics.hpp>
struct Box
{
	sf::Vector2f position;
	bool isStateFinal;
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
	Box(const sf::Vector2f &position, bool isState);
};

