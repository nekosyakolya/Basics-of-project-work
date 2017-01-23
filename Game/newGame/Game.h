#pragma once

#include "level.h"
#include "Player.h"

class CGame
{
public:
	CGame();
	void Initialisation();

	Level GetLevel();
	CPlayer GetPlayer();

	void UpdatePlayer(float);


	sf::View view;



	~CGame();
private:
	Level m_level;
	CPlayer m_player;
};

