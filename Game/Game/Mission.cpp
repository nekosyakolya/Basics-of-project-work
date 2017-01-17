#include "stdafx.h"
#include "Mission.h"



CMission::CMission(const sf::Vector2f & position):
CBonus(position)
{
	m_image.loadFromFile("resources/spritesheet.png");

	auto positionSprite = sf::Vector2f(192, 32);

	m_texture.loadFromImage(m_image);

	m_sprite.setTexture(m_texture);



	m_sprite.setTextureRect(sf::IntRect(static_cast<int>(positionSprite.x), static_cast<int>(positionSprite.y), 32, 32));


}



CMission::~CMission()
{
}
