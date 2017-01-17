#include "stdafx.h"
#include "Puddle.h"


CPuddle::CPuddle(const sf::Vector2f &position):
CBonus(position)
{
	m_image.loadFromFile("resources/map.png");
	auto positionSprite = sf::Vector2f(0, 240);


	m_texture.loadFromImage(m_image);

	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(sf::IntRect(static_cast<int>(positionSprite.x), static_cast<int>(positionSprite.y), 58, 32));
	m_buffer.loadFromFile("resources/puddle.wav");
	m_sound.setBuffer(m_buffer);
	m_sound.setVolume(40);
	m_visited = false;
}

void CPuddle::PlaySound()
{
	m_sound.play();
}

void CPuddle::SetVisited()
{
	m_visited = true;
}

bool CPuddle::IsVisited() const
{
	return m_visited;
}

CPuddle::~CPuddle()
{
}
