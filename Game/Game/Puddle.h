#pragma once

#include "bonus.h"

class CPuddle:
	public CBonus
{
public:
	CPuddle(const sf::Vector2f &);
	void PlaySound();
	void SetVisited();
	bool IsVisited() const;


	~CPuddle();
private:
	sf::SoundBuffer m_buffer;
	sf::Sound m_sound;
	bool m_visited;
};

