#pragma once
#include <SFML/Audio.hpp>

#include "level.h"
#include "Hero.h"



class CPlayer : public
	CHero
{
public:
	CPlayer(sf::Vector2f);

	void GetDirection();
	void Update(float time) override;
	
	void ChangePosition();
	void UpdatePosition(float dx) override;
	

	void SetPlaceInFinal();
	bool IsJump();
	void PlaySound();
	unsigned GetPlaceInFinal() const;
	bool IsNewMission()const;

	void SetNewMission();
	void UpdateDelta();

	void SetSpeed();

	sf::Sprite GetSpriteProtection()const;
	sf::Sprite GetSpriteFreeze()const;

	void InitClock();
	void SetDelay();

	void Freezing();
	void SetFreeze();

	int GetTotal();
	void UpdateTotal(int);

	bool IsFreeze()const;

	~CPlayer();
private:
	unsigned m_placeInFinal;

	bool m_isNewMission;
	sf::SoundBuffer m_bufferJump;
	sf::Sound m_soundJump;
	sf::SoundBuffer m_buffer;
	sf::Sound m_sound;


	sf::Image m_imageProtection;
	sf::Texture m_textureProtection;
	sf::Sprite m_spriteProtection;

	sf::Texture m_textureFreeze;
	sf::Sprite m_spriteFreeze;


	sf::Clock m_clock;
	unsigned m_time;
	bool m_delay;

	sf::Clock m_clockTotal;
	unsigned m_timeTotal;
	int m_total;

	bool m_freeze;
};

