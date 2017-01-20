#pragma once

#include "level.h"

class CGame
{
public:
	CGame();
	void Init();

	Level GetLevel();
	~CGame();
private:
	Level m_level;
};

