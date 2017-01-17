#pragma once

#include "bonus.h"

class CMission:
	public CBonus
{
public:
	CMission(const sf::Vector2f &);

	~CMission();
};

