#pragma once

#include "level.h"
#include "Player.h"
#include "Donut.h"
#include "Enemy.h"

#include <memory>


class CGame
{
public:
	CGame();
	void Initialisation();

	Level GetLevel()const;
	CPlayer GetPlayer()const;


	void Update(float);

	void Display(sf::RenderWindow &)const;

	~CGame();
private:
	Level m_level;
	CPlayer m_player;
	sf::View m_view;
	
	std::vector<std::shared_ptr<CDonut>> m_bonuses;
	std::vector<std::shared_ptr<CEnemy>> m_enemies;

	void CheckCollisionWithDonuts();
	void UpdatePlayer(float);

	void DisplayDonuts(sf::RenderWindow &)const;

};

