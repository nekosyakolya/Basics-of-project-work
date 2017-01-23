#pragma once

#include "level.h"
#include "Player.h"
#include "Donut.h"

#include <memory>


class CGame
{
public:
	CGame();
	void Initialisation();

	Level GetLevel();
	CPlayer GetPlayer();


	void Update(float);

	void Display(sf::RenderWindow &);

	~CGame();
private:
	Level m_level;
	CPlayer m_player;
	sf::View m_view;
	void UpdatePlayer(float);
	std::vector<std::shared_ptr<CDonut>> m_bonuses;

};

