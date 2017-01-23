#include "stdafx.h"
#include "Game.h"


CGame::CGame()
{

}

void CGame::Initialisation()
{

	m_view.reset(sf::FloatRect(0, 0, 680, 500));

	m_level.LoadFromFile("resources/map.tmx");

	Object hero = m_level.GetObject("player");
	m_player.Init(m_level);

	m_player.InitPlayer(sf::Vector2f(hero.rect.left, hero.rect.top));


	std::vector<Object> bonusStartPos = m_level.GetObjects("bonus");

	for (auto currBonus : bonusStartPos)
	{
		m_bonuses.push_back(std::shared_ptr<CDonut>(new CDonut(sf::Vector2f(currBonus.rect.left, currBonus.rect.top))));
	}

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

}

void CGame::Update(float time)
{
	UpdatePlayer(time);
	

	for (size_t i = 0; i < m_bonuses.size(); ++i)
	{
		if (m_player.GetRect().intersects(m_bonuses[i]->GetSprite().getGlobalBounds()))
		{
			m_bonuses.erase(m_bonuses.begin() + i);
			m_player.PlaySound();
			m_player.ChangePosition();
			m_player.UpdateTotal(5);
		}
	}


	m_player.SetPosition();
	m_player.SetSpeed();
	m_player.Freezing();


	m_view.setCenter(340, GetPlayer().GetPosition().y - 213);


}

void CGame::Display(sf::RenderWindow & window)
{
	window.setView(m_view);

	window.clear(sf::Color::White);
	GetLevel().Draw(window);

	for (auto bonus : m_bonuses)
	{
		window.draw(bonus->GetSprite());
	}

	window.draw(GetPlayer().GetHero());
	window.display();
}

CGame::~CGame()
{
}
