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



	std::vector<Object> enemyStartPos = m_level.GetObjects("enemy");
	sf::Clock clock;
	for (auto currEnemy : enemyStartPos)
	{
		float timeSpeed = static_cast<float>(clock.getElapsedTime().asMicroseconds());
		clock.restart();
		timeSpeed = timeSpeed / 10000000;
		m_enemies.push_back(std::shared_ptr<CEnemy>(new CEnemy(sf::Vector2f(currEnemy.rect.left, currEnemy.rect.top), timeSpeed)));
	}


	for (auto enemy : m_enemies)
	{
		enemy->Init(m_level);
	}

}



Level CGame::GetLevel()const
{
	return m_level;
}

CPlayer CGame::GetPlayer()const
{
	return m_player;
}


void CGame::UpdatePlayer(float time)
{
	m_player.Update(time);

}

void CGame::CheckCollisionWithDonuts()
{
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
}

void CGame::DisplayDonuts(sf::RenderWindow & window)const
{
	for (auto bonus : m_bonuses)
	{
		window.draw(bonus->GetSprite());
	}
}

void CGame::Update(float time)
{
	UpdatePlayer(time);
	

	for (auto enemy : m_enemies)
	{
		enemy->Update(time);

		if (m_player.GetRect().intersects(enemy->GetHero().getGlobalBounds()))
		{
			if (m_player.GetOffset().x < 0)
			{
				m_player.UpdatePosition(enemy->GetPosition().x + 40);
				enemy->UpdatePosition(-0.1f * time);
			}
			if (m_player.GetOffset().x > 0)
			{
				m_player.UpdatePosition(enemy->GetPosition().x - 40);
				enemy->UpdatePosition(0.1f * time);
			}


			if (m_player.GetOffset().x == 0)
			{
				if (!enemy->Check(0.1f * time))
				{
					enemy->Check(-0.1f * time);
				}

			}
		}


		for (size_t i = 0; i < m_bonuses.size(); ++i)
		{
			if (enemy->GetRect().intersects(m_bonuses[i]->GetSprite().getGlobalBounds()))
			{
				m_bonuses.erase(m_bonuses.begin() + i);
			}
		}

		enemy->SetPosition();
	}



	CheckCollisionWithDonuts();


	m_player.SetPosition();
	m_player.SetSpeed();
	m_player.Freezing();


	m_view.setCenter(340, GetPlayer().GetPosition().y - 213);


}

void CGame::Display(sf::RenderWindow & window)const
{
	window.setView(m_view);

	window.clear(sf::Color::White);
	GetLevel().Draw(window);

	DisplayDonuts(window);


	for (auto enemy : m_enemies)
	{
		window.draw(enemy->GetHero());

		if (enemy->IsJump())
		{
			window.draw(enemy->GetRectProtection());
		}
	}



	window.draw(GetPlayer().GetHero());

	if (GetPlayer().IsJump())
	{
		window.draw(m_player.GetSpriteProtection());
	}

	if (m_player.IsFreeze())
	{
		window.draw(m_player.GetSpriteFreeze());
	}




	window.display();
}

CGame::~CGame()
{
}
