#include "stdafx.h"
#include "Donut.h"




CDonut::CDonut(const sf::Vector2f & position):
CBonus(position)
{
	m_image.loadFromFile("resources/map.png");
	auto positionSprite = sf::Vector2f(0, 120);


	m_texture.loadFromImage(m_image);

	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(sf::IntRect(static_cast<int>(positionSprite.x), static_cast<int>(positionSprite.y), 40, 40));

}

CDonut::~CDonut()
{
}
