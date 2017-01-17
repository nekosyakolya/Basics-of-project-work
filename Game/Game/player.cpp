#include "stdafx.h"
#include "player.h"


CPlayer::CPlayer(sf::Vector2f position) :
	CHero(position)
{
	m_delta = -0.1f;
	m_currentFrame = 0.0;
	m_time = 0;

	m_total = 0;
	m_freeze = false;
	m_buffer.loadFromFile("resources/eating.wav");
	m_sound.setBuffer(m_buffer);


	m_bufferJump.loadFromFile("resources/jump.wav");
	m_soundJump.setBuffer(m_bufferJump);


	m_isNewMission = false;
	m_placeInFinal = 0;

	m_delay = false;

	m_imageProtection.loadFromFile("resources/map.png");

	auto positionSpriteProtection = sf::Vector2f(0, 273);
	m_textureProtection.loadFromImage(m_imageProtection);

	m_spriteProtection.setTexture(m_textureProtection);

	m_spriteProtection.setTextureRect(sf::IntRect(static_cast<int>(positionSpriteProtection.x), static_cast<int>(positionSpriteProtection.y), 50, 50));



	auto positionSpriteFreeze = sf::Vector2f(0, 320);


	m_textureFreeze.loadFromImage(m_imageProtection);

	m_spriteFreeze.setTexture(m_textureFreeze);
	m_spriteFreeze.setTextureRect(sf::IntRect(static_cast<int>(positionSpriteFreeze.x), static_cast<int>(positionSpriteFreeze.y), 40, 40));


	auto positionSprite = sf::Vector2f(0, 120);


	m_texture.loadFromImage(m_image);

	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(sf::IntRect(static_cast<int>(positionSprite.x), static_cast<int>(positionSprite.y), 40, 40));


	m_clockTotal.restart();
	m_timeTotal = static_cast<unsigned>(m_clockTotal.getElapsedTime().asSeconds());


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


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		m_direction = Direction::JUMP;
	}

}

void CPlayer::Update(float time)
{
	m_currentFrame += 0.005f * time;
	if (m_currentFrame > 3)
	{
		m_currentFrame = 0.0;
	}


	GetDirection();
	switch (m_direction)
	{
	case Direction::RIGHT:
		m_offset.x = 0.1f;
		break;
	case Direction::LEFT:
		m_offset.x = -0.1f;
		break;
	case Direction::UP:
		m_offset.x = 0;
		break;
	case Direction::JUMP:
		m_offset.y = -0.7f;
		m_soundJump.play();
		break;
	default:
		break;
	}

	if (IsFinalState())
	{
		m_delta /= 1.014f;
	}
	else if (!m_freeze)
	{
		m_sprite.setTextureRect(sf::IntRect(40, 40 * static_cast<int>(m_currentFrame), 40, 40));
	}
	m_offset.y = m_delta;

	m_position.y += m_offset.y * time;

	m_position.x += m_offset.x * time;
	CheckCollision(m_offset.x, 0);

	if (IsJump())
	{
		m_spriteProtection.setPosition(m_position.x - 5, m_position.y - 5);
	}

	if (m_timeTotal > 0.5 && !IsFinalState())
	{
		m_clockTotal.restart();
		++m_total;
	}
	m_timeTotal = static_cast<unsigned>(m_clockTotal.getElapsedTime().asSeconds());

}



void CPlayer::ChangePosition()
{
	m_delta /= 1.02f;
}



void CPlayer::UpdatePosition(float dx)
{
	m_position.x = dx;
}



sf::Sprite CPlayer::GetSpriteProtection() const
{
	return m_spriteProtection;
}

sf::Sprite CPlayer::GetSpriteFreeze() const
{
	return m_spriteFreeze;
}

void CPlayer::InitClock()
{
	m_clock.restart();
	m_time = static_cast<unsigned>(m_clock.getElapsedTime().asSeconds());

}

void CPlayer::SetDelay()
{
	m_delay = true;
}

void CPlayer::Freezing()
{
	if (m_freeze)
	{
		m_spriteFreeze.setPosition(m_position.x, m_position.y);
		m_delta = 0;
		if (m_time >= 2)
		{
			m_clock.restart();
			m_freeze = false;
			m_delta = -0.1f;
			m_offset.x = 0;
		}
		m_time = static_cast<unsigned>(m_clock.getElapsedTime().asSeconds());
	}
}

void CPlayer::SetFreeze()
{
	m_freeze = true;
}

int CPlayer::GetTotal()
{
	return m_total;
}

void CPlayer::UpdateTotal(int dTotal)
{
	m_total += dTotal;
	if (m_total < 0)
	{
		m_total = 0;
	}
}

bool CPlayer::IsFreeze() const
{
	return m_freeze;
}

CPlayer::~CPlayer()
{
}


void CPlayer::SetPlaceInFinal()
{
	++m_placeInFinal;
}

bool CPlayer::IsJump()
{
	return m_direction == Direction::JUMP;
}


void CPlayer::PlaySound()
{
	m_sound.play();
}

unsigned CPlayer::GetPlaceInFinal() const
{
	return m_placeInFinal;
}

bool CPlayer::IsNewMission() const
{
	return m_isNewMission;
}

void CPlayer::SetNewMission()
{
	if (!m_isNewMission)
	{
		m_isNewMission = true;
	}
	else
	{
		m_isNewMission = false;
	}

}

void CPlayer::UpdateDelta()
{
	m_delta *= 1.04f;
}

void CPlayer::SetSpeed()
{
	if (m_delay)
	{
		m_delta -= 0.0005f;
		if (m_time >= 2)
		{
			m_clock.restart();
			m_delay = false;
			m_delta = -0.1f;
		}
		m_time = static_cast<unsigned>(m_clock.getElapsedTime().asSeconds());
	}

}
