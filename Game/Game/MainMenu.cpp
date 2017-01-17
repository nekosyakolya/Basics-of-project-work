#include "stdafx.h"
#include "MainMenu.h"


CMainMenu::CMainMenu()
{
	m_isOpen = true;
	m_currentFrame = 0.0;
	m_timeAnimation = static_cast<unsigned>(m_clock.getElapsedTime().asSeconds());

	m_texture.loadFromFile("resources/menu.jpg");
	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(0, 0);

	m_textureAnimation.loadFromFile("resources/eye.png");
	m_spriteAnimation.setTexture(m_textureAnimation);
	m_spriteAnimation.setTextureRect(sf::IntRect(0, 0, 100, 16));
	m_spriteAnimation.setPosition(256, 136);


	m_textureButton.loadFromFile("resources/play.png");
	m_spriteButton.setTexture(m_textureButton);
	m_spriteButton.setPosition(550, 30);
	m_spriteButton.setScale(sf::Vector2f(0.3f, 0.3f));

	m_music.openFromFile("resources/play.wav");
	m_music.play();
	m_music.setLoop(true);
}

bool CMainMenu::IsOpen() const
{
	return m_isOpen;
}

sf::Sprite CMainMenu::GetSprite() const
{
	return m_sprite;
}

sf::Sprite CMainMenu::GetSpriteAnimation() const
{
	return m_spriteAnimation;
}

sf::Sprite CMainMenu::GetButton() const
{
	return m_spriteButton;
}

void CMainMenu::Animation(float time)
{

	if (m_timeAnimation >= 3)
	{
		m_currentFrame += 0.007f * time;
		if (m_currentFrame > 4)
		{
			m_currentFrame = 0;
			m_clock.restart();
		}
		m_spriteAnimation.setTextureRect(sf::IntRect(0, 16 * static_cast<int>(m_currentFrame), 100, 16));

	}

	m_timeAnimation = static_cast<unsigned>(m_clock.getElapsedTime().asSeconds());
}

void CMainMenu::SetOpen()
{
	m_isOpen = false;

	m_music.stop();
}


CMainMenu::~CMainMenu()
{
}
