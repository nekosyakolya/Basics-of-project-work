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

	void SetBigSpeed();

	sf::Sprite GetSpriteProtection()const;

	void InitClock();

	unsigned GetTotal();
	void UpdateTotal(int);

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


	sf::Clock m_clock;
	unsigned m_time;
	bool m_delay;

	sf::Clock m_clockTotal;
	unsigned m_timeTotal;
	unsigned m_total;
};

