#pragma once
#include <SFML/Graphics.hpp>
#include "level.h"


class CBonus
{
public:
	CBonus(sf::Vector2f);
	sf::Sprite GetSprite();
	~CBonus();
private:

	sf::Vector2f m_position;
	sf::Image m_image;
	sf::Texture m_texture;
	sf::Sprite m_sprite;
};
