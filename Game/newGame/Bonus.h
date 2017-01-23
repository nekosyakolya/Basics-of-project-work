#pragma once
#include <SFML/Graphics.hpp>
#include "level.h"


class CBonus
{
public:
	CBonus(const sf::Vector2f &);
	sf::Sprite GetSprite() const;
	virtual ~CBonus();
protected:
	sf::Vector2f m_position;
	sf::Image m_image;
	sf::Texture m_texture;
	sf::Sprite m_sprite;
};

