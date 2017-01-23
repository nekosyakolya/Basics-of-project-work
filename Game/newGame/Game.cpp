#include "stdafx.h"
#include "Game.h"


CGame::CGame()
{

}

void CGame::Initialisation()
{
	m_level.LoadFromFile("resources/map.tmx");

	Object hero = m_level.GetObject("player");
	m_player.Init(m_level);

	m_player.InitPlayer(sf::Vector2f(hero.rect.left, hero.rect.top));

}



Level CGame::GetLevel()
{
	return m_level;
}

CPlayer CGame::GetPlayer()
{
	return m_player;
}

void CGame::UpdatePlayer(float time)
{
	m_player.Update(time);
	m_player.SetPosition();
	m_player.SetSpeed();
	m_player.Freezing();

}

CGame::~CGame()
{
}
