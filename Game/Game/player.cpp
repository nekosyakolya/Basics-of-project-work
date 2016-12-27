#include "stdafx.h"
#include "player.h"


CPlayer::CPlayer(sf::Vector2f position):
	m_object(sf::Vector2f(40, 40)),
	m_position(position),
	m_direction(Direction::UP),
	m_offset(sf::Vector2f(0, 0))
{
	m_object.setPosition(m_position);
	m_object.setFillColor(sf::Color::Red);
}

sf::RectangleShape CPlayer::GetPlayer() const
{
	return m_object;
}

sf::Vector2f CPlayer::GetPosition() const
{
	return m_position;
}


void CPlayer::GetDirection()
{
	m_direction = Direction::UP;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_direction = Direction::LEFT;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_direction = Direction::RIGHT;
	}
}

void CPlayer::Update(float time)
{

	GetDirection();
	switch (m_direction)
	{
	case Direction::RIGHT:
		m_offset.x = 0.1;
		break;
	case Direction::LEFT:
		m_offset.x = -0.1;
		break;
	case Direction::UP:
		m_offset.x = 0;
		break;
	default:
		break;
	}
	m_offset.y = -0.1;
	m_position.x += m_offset.x * time;
	CheckCollision(m_offset.x, 0);

	m_position.y += m_offset.y * time;

	m_object.setPosition(m_position);
}

void CPlayer::CheckCollision(float dx, float dy)
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

void CPlayer::InitPlayer(Level & level)
{
	m_obj = level.GetAllObjects();
}


CPlayer::~CPlayer()
{
}

sf::FloatRect CPlayer::GetRect()
{

	return sf::FloatRect(m_position.x, m_position.y, 40, 40);
}
