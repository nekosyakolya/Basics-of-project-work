#include "stdafx.h"
#include "Enemy.h"


CEnemy::CEnemy(const sf::Vector2f &position, float delta)
{
	InitHero(position);
	m_image.loadFromFile("resources/slon.png");


	m_direction = Direction::UP;
	m_offset = sf::Vector2f(0, 0);
	m_isFinalState = false;


	m_delta = -0.1f - (0.01f * delta);
	auto positionSprite = sf::Vector2f(0, 0);


	m_texture.loadFromImage(m_image);

	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(sf::IntRect(static_cast<int>(positionSprite.x), static_cast<int>(positionSprite.y), 40, 40));
	m_currentFrame = 0.0;

	m_imageProtection.loadFromFile("resources/map.png");

	auto positionSpriteProtection = sf::Vector2f(0, 273);
	m_textureProtection.loadFromImage(m_imageProtection);

	m_spriteProtection.setTexture(m_textureProtection);

	m_spriteProtection.setTextureRect(sf::IntRect(static_cast<int>(positionSpriteProtection.x), static_cast<int>(positionSpriteProtection.y), 50, 50));

	m_isJump = false;
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



	m_isJump = false;
	m_spriteProtection.setPosition(m_position.x, m_position.y);
}


void CEnemy::UpdatePosition(float dx)
{
	m_position.x += dx;


	CheckCollision(dx, 0);


}

bool CEnemy::Check(float dx)
{
	m_position.x += dx;
	for (size_t i = 0; i < m_obj.size(); ++i)
	{

		if (GetRect().intersects(m_obj[i].rect) && (m_obj[i].name == "collision"))
		{
			if (dx > 0)
			{
				m_position.x = m_obj[i].rect.left - 40 * 2;
				return false;
			}
			if (dx < 0)
			{
				m_position.x = m_obj[i].rect.left + (m_obj[i].rect.width * 2);
				return false;

			}
		}
	}
	return true;
}

sf::Sprite CEnemy::GetRectProtection() const
{
	return m_spriteProtection;
}



void CEnemy::SetProtection()
{
	m_isJump = true;


}

bool CEnemy::IsJump() const
{
	return m_isJump;
}

CEnemy::~CEnemy()
{
}



