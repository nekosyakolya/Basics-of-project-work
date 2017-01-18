#include "stdafx.h"
#include "bonus.h"

CBonus::CBonus(const sf::Vector2f & position):
	m_position(position)
{
	//m_image.loadFromFile("resources/map.png");

	//auto positionSprite = sf::Vector2f(0, 120);
	//m_texture.loadFromImage(m_image);

	//m_sprite.setTexture(m_texture);

	//m_sprite.setTextureRect(sf::IntRect(static_cast<int>(positionSprite.x), static_cast<int>(positionSprite.y), 40, 40));

	m_sprite.setPosition(m_position);

}

sf::Sprite CBonus::GetSprite() const
{
	return m_sprite;
}

CBonus::~CBonus()
{
}