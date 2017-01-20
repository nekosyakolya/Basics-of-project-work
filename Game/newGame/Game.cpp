#include "stdafx.h"
#include "Game.h"


CGame::CGame()
{
}

void CGame::Init()
{
	m_level.LoadFromFile("resources/map.tmx");
}


Level CGame::GetLevel()
{
	return m_level;
}

CGame::~CGame()
{
}
