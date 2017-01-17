#include "stdafx.h"

CEnemy::CEnemy(const sf::Vector2f &position, float delta):
	CHero(position)
{
	m_delta = -0.1f - (0.01f * delta);
	auto positionSprite = sf::Vector2f(0, 0);


	m_texture.loadFromImage(m_image);

	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(sf::IntRect(static_cast<int>(positionSprite.x), static_cast<int>(positionSprite.y), 40, 40));
	m_currentFrame = 0.0;
}

void CEnemy::Update(float time)
{
	m_currentFrame += 0.005f * time;
	if (m_currentFrame > 3)
	{
		m_currentFrame = 0.0;
	}

	if (IsFinalState())
	{
		m_delta /= 1.014f;
	}
	else
	{
		m_sprite.setTextureRect(sf::IntRect(0, 40 * static_cast<int>(m_currentFrame), 40, 40));

	}
	m_offset.y = m_delta;

	m_position.y += m_offset.y * time;
}


void CEnemy::UpdatePosition(float dx)
{
	m_position.x += dx;


	CheckCollision(dx, 0);
}



CEnemy::~CEnemy()
{
}


