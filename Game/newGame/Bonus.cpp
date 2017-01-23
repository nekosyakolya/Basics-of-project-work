#include "stdafx.h"
#include "Bonus.h"

#include "stdafx.h"
#include "bonus.h"

CBonus::CBonus(const sf::Vector2f & position) :
	m_position(position)
{
	m_sprite.setPosition(m_position);

}

sf::Sprite CBonus::GetSprite() const
{
	return m_sprite;
}

CBonus::~CBonus()
{
}