#include "stdafx.h"
#include "Turtle.h"



bool CTurtle::GetDelay() const
{
	return m_delay;
}

sf::Sprite CTurtle::GetAnimation()
{
	return m_spriteAnimation;
}

CTurtle::~CTurtle()
{
}



CTurtle::CTurtle(const sf::Vector2f &position) :
	m_position(position),
	m_direction(Direction::UP),
	m_offset(sf::Vector2f(0, 0))
{
	m_sprite.setPosition(m_position);
	m_image.loadFromFile("resources/turtle.png");
	m_currentFrame = 0.0;


	auto positionSprite = sf::Vector2f(0, 0);


	m_texture.loadFromImage(m_image);

	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(sf::IntRect(static_cast<int>(positionSprite.x), static_cast<int>(positionSprite.y), 40, 40));




	m_spriteBonus.setTexture(m_texture);

	auto positionSpriteBonus = sf::Vector2f(0, 80);

	m_spriteBonus.setTextureRect(sf::IntRect(static_cast<int>(positionSpriteBonus.x), static_cast<int>(positionSpriteBonus.y), 40, 40));

	m_imageAnimation.loadFromFile("resources/turtle-frame.png");
	m_textureAnimation.loadFromImage(m_imageAnimation);

	m_spriteAnimation.setTexture(m_textureAnimation);



	m_row = 0;
	m_isCollision = false;
	m_isShow = false;
	m_delay = false;
}

sf::Sprite CTurtle::GetHero() const
{
	return m_sprite;
}

sf::Vector2f CTurtle::GetPosition() const
{
	return m_position;
}

sf::Vector2f CTurtle::GetOffset() const
{
	return m_offset;
}

sf::FloatRect CTurtle::GetRect() const
{
	return sf::FloatRect(m_position.x, m_position.y, 40, 40);
}

sf::FloatRect CTurtle::GetRectBonus() const
{
	return sf::FloatRect(m_objBonus.rect.left - 40, m_objBonus.rect.top, 40, 40);
}

sf::Sprite CTurtle::GetBonus() const
{
	return m_spriteBonus;
}

void CTurtle::Init(Level & level)
{
	m_obj = level.GetAllObjects();
	m_objBonus = level.GetObject("turtleBonuses");
	m_spriteBonus.setPosition(m_objBonus.rect.left - 40, m_objBonus.rect.top);

}

void CTurtle::SetPosition()
{
	m_sprite.setPosition(m_position);
}

void CTurtle::SetShow()
{
	m_isShow = false;
}

bool CTurtle::IsStop(float playerPosition)
{
	return !(playerPosition - m_position.y <= 280 && !m_isCollision);
}

bool CTurtle::IsRun()
{
	return m_isShow && !m_isCollision;
}

void CTurtle::Update(float time)
{
	m_currentFrame += 0.004f * time;

	if (m_currentFrame > 3)
	{
		m_currentFrame = 0.0;
		m_row = 1;
		m_offset.x = 0.2f;
	}


	m_position.x += m_offset.x * time;
	CheckCollision(m_offset.x, 0);

	m_sprite.setTextureRect(sf::IntRect(40 * static_cast<int>(m_currentFrame), m_row * 40, 40, 40));
}

void CTurtle::SetDelay()
{
	m_delay = true;
}

void CTurtle::UpdateDelay()
{
	if (m_delay)
	{
		if (m_time >= 3)
		{
			m_clock.restart();
			m_delay = false;
		}
		m_time = static_cast<unsigned>(m_clock.getElapsedTime().asSeconds());
	}
}

void CTurtle::InitClock()
{
	m_clock.restart();
	m_time = static_cast<unsigned>(m_clock.getElapsedTime().asSeconds());

}


bool CTurtle::IsShow()
{
	return m_isShow;
}


void CTurtle::CheckCollision(float dx, float /*dy*/)
{


	for (size_t i = 0; i < m_obj.size(); ++i)
	{

		if (GetRect().intersects(m_obj[i].rect) && (m_obj[i].name == "collision"))
		{
			if (dx > 0)
			{
				m_position.x = m_obj[i].rect.left - 40;
				m_isCollision = true;
			}
		}
	}
	if (GetRect().intersects(m_objBonus.rect))
	{
		m_isShow = true;
	}
}



