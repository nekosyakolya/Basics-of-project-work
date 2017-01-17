#pragma once


#include "Hero.h"


class CEnemy: public
	CHero
{
public:
	CEnemy(const sf::Vector2f &position, float delta);
	void Update(float time) override;

	
	void UpdatePosition(float dx) override;
	
	~CEnemy();
};

