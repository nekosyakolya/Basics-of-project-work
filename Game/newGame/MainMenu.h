#pragma once
class CMainMenu
{
public:
	CMainMenu();

	bool IsOpen() const;

	sf::Sprite GetSprite() const;

	sf::Sprite GetSpriteAnimation() const;

	sf::Sprite GetSpriteEnd() const;

	sf::Sprite GetButton() const;

	void Animation(float time);

	void SetOpen();

	void End();

	bool IsEnd();

	~CMainMenu();
private:
	bool m_isOpen;
	float m_currentFrame;

	sf::Clock m_clock;
	unsigned m_timeAnimation;


	sf::Texture m_texture;

	sf::Sprite m_sprite;


	sf::Texture m_textureAnimation;

	sf::Sprite m_spriteAnimation;


	sf::Sprite m_spriteButton;

	sf::Texture m_textureButton;


	sf::Sprite m_spriteEnd;

	sf::Texture m_textureEnd;

	sf::Music m_music;


	bool m_end;
};

