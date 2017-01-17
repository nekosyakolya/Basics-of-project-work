#include "stdafx.h"


CHero::CHero(const sf::Vector2f &position):

	m_position(position),
	m_direction(Direction::UP),
	m_offset(sf::Vector2f(0, 0))
{
	m_sprite.setPosition(m_position);
	m_image.loadFromFile("resources/slon.png");
}

sf::Sprite CHero::GetHero() const
{
	return m_sprite;
}


sf::Vector2f CHero::GetPosition() const
{
	return m_position;
}

sf::Vector2f CHero::GetOffset() const
{
	return m_offset;
}

sf::FloatRect CHero::GetRect() const
{
	return sf::FloatRect(m_position.x, m_position.y, 40, 40);
}

void CHero::SetPosition()
{
	m_sprite.setPosition(m_position);
}

bool CHero::IsFinalState() const
{
	return m_isFinalState;
}

void CHero::SetFinalState()
{
	m_isFinalState = true;
}

void CHero::Init(Level & level)
{
	m_obj = level.GetAllObjects();
}

CHero::~CHero()
{
}

void CHero::CheckCollision(float dx, float dy)
{

	for (size_t i = 0; i < m_obj.size(); ++i)
	{

		if (GetRect().intersects(m_obj[i].rect) && (m_obj[i].name == "collision"))
		{
			if (dx > 0)
			{
				m_position.x = m_obj[i].rect.left - 40;
			}
			if (dx < 0)
			{
				m_position.x = m_obj[i].rect.left + m_obj[i].rect.width;
			}
		}
	}
}

