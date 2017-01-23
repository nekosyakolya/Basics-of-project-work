#include "stdafx.h"
#include "Game.h"


CGame::CGame()
{

}

void CGame::Initialisation()
{
	m_font.loadFromFile("resources/CyrilicOld.TTF");
	m_text.setString("");
	m_text.setFont(m_font);
	m_text.setCharacterSize(32);
	m_text.setFillColor(sf::Color::White);
	m_text.setStyle(sf::Text::Bold);
	m_text.setPosition(40, 20);



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



	std::vector<Object> puddleStartPos = m_level.GetObjects("puddle");

	for (auto currPuddle : puddleStartPos)
	{
		m_puddles.push_back(std::shared_ptr<CPuddle>(new CPuddle(sf::Vector2f(currPuddle.rect.left, currPuddle.rect.top))));
	}


	m_finish = m_level.GetObject("finish");



	std::vector<Object> newMissionStart = m_level.GetObjects("newMission");

	for (auto currBonus : newMissionStart)
	{
		m_scrolls.push_back(std::shared_ptr<CScroll>(new CScroll(sf::Vector2f(currBonus.rect.left, currBonus.rect.top))));
	}



	std::vector<Object> elephantStartPos = m_level.GetObjects("elephant");

	for (auto currElephant : elephantStartPos)
	{
		m_elephants.push_back(std::shared_ptr<CElephant>(new CElephant(sf::Vector2f(currElephant.rect.left, currElephant.rect.top))));
	}

	for (auto currElephant : m_elephants)
	{
		currElephant->Init(m_level);
	}


	std::vector<Object> catStartPos = m_level.GetObjects("blackCat");
	for (auto currCat : catStartPos)
	{
		m_cats.push_back(std::shared_ptr<CCat>(new CCat(sf::Vector2f(currCat.rect.left, currCat.rect.top))));
	}

	for (auto cat : m_cats)
	{
		cat->Init(m_level);
	}



	m_frame.textureFrame.loadFromFile("resources/frame.png");
	m_frame.spriteFrame.setTexture(m_frame.textureFrame);
	m_frame.spriteFrame.setPosition(0, m_player.GetPosition().y - 470);

	m_frame.textureButton.loadFromFile("resources/frame-next.png");
	m_frame.spriteButton.setTexture(m_frame.textureButton);
	m_frame.spriteButton.setPosition(440, m_player.GetPosition().y - 460);

	m_frame.text.setString("");
	m_frame.text.setFont(m_font);
	m_frame.text.setCharacterSize(24);
	m_text.setFillColor(sf::Color::White);
	m_text.setStyle(sf::Text::Bold);
	m_text.setPosition(20, m_player.GetPosition().y - 460);

	m_music.openFromFile("resources/main.wav");
	m_music.setVolume(10.5f);
	m_music.setLoop(true);

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

void CGame::CheckCollisionWithPuddles()
{

	for (auto puddle : m_puddles)
	{
		if (m_player.GetRect().intersects(puddle->GetSprite().getGlobalBounds()) &&
			!m_player.IsJump() && !puddle->IsVisited())
		{
			m_player.ChangePosition();
			m_player.UpdateTotal(-5);
			puddle->PlaySound();
			puddle->SetVisited();
		}
	}
}

void CGame::DrawDonuts(sf::RenderWindow & window)const
{
	for (auto bonus : m_bonuses)
	{
		window.draw(bonus->GetSprite());
	}
}

void CGame::DrawEnemies(sf::RenderWindow &window) const
{

	for (auto enemy : m_enemies)
	{
		window.draw(enemy->GetHero());

		if (enemy->IsJump())
		{
			window.draw(enemy->GetRectProtection());
		}
	}

}

void CGame::DrawPuddles(sf::RenderWindow &window) const
{
	for (auto puddle : m_puddles)
	{
		window.draw(puddle->GetSprite());
	}
}

void CGame::DrawScrolls(sf::RenderWindow &window) const
{
	for (auto scroll : m_scrolls)
	{
		window.draw(scroll->GetSprite());
	}
}

void CGame::Update(float time)
{

	if (!m_menu.IsOpen())
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


			if (m_finish.rect.intersects(enemy->GetHero().getGlobalBounds()) && !enemy->IsFinalState())
			{
				enemy->SetFinalState();
				m_player.SetPlaceInFinal();

			}


			for (size_t i = 0; i < m_bonuses.size(); ++i)
			{
				if (enemy->GetRect().intersects(m_bonuses[i]->GetSprite().getGlobalBounds()))
				{
					m_bonuses.erase(m_bonuses.begin() + i);
				}
			}


			for (auto elephant : m_elephants)
			{
				if ((enemy->GetRect().intersects(elephant->GetRectBonus()) && elephant->IsShow()) || enemy->GetRect().intersects(elephant->GetRect()))
				{
					enemy->SetProtection();
				}
			}

			for (auto scroll : m_scrolls)
			{
				if (enemy->GetRect().intersects(scroll->GetSprite().getGlobalBounds()))
				{
					enemy->SetProtection();
				}
			}

			for (auto cat : m_cats)
			{
				if ((enemy->GetRect().intersects(cat->GetRectBonus()) && cat->IsShow()) || enemy->GetRect().intersects(cat->GetRect()))
				{
					enemy->SetProtection();
				}

			}

			enemy->SetPosition();
		}


		for (auto elephant : m_elephants)
		{
			if (!(elephant->IsStop(m_player.GetPosition().y)))
			{
				elephant->Update(time);
				elephant->SetPosition();
			}

			if (m_player.GetRect().intersects(elephant->GetRectBonus()) && elephant->IsShow())
			{
				elephant->SetShow();
				m_player.InitClock();
				m_player.SetDelay();
				m_player.UpdateTotal(100);
			}

		}


		if (m_player.GetRect().intersects(m_finish.rect) && !m_player.IsFinalState())
		{
			m_player.SetFinalState();
			m_player.SetPlaceInFinal();
			m_player.UpdateTotal(210 * 1 / m_player.GetPlaceInFinal());
			m_text.setString("Вы заняли " + std::to_string(m_player.GetPlaceInFinal()) + " место\n");
		}

		for (size_t i = 0; i < m_scrolls.size(); ++i)
		{
			if (m_player.GetRect().intersects(m_scrolls[i]->GetSprite().getGlobalBounds()))
			{
				m_scrolls.erase(m_scrolls.begin() + i);
				m_player.SetNewMission();
				//mission.PlaySound();
			}
		}



		for (auto cat : m_cats)
		{
			if (!(cat->IsStop(m_player.GetPosition().y)))
			{
				cat->Update(time);
				cat->SetPosition();
			}

			if (m_player.GetRect().intersects(cat->GetRectBonus()) && cat->IsShow())
			{
				cat->SetShow();
				m_player.InitClock();
				m_player.SetFreeze();
			}

		}


		CheckCollisionWithDonuts();


		CheckCollisionWithPuddles();

		m_player.SetPosition();
		m_player.SetSpeed();
		m_player.Freezing();


		m_text.setPosition(180, m_player.GetPosition().y - 240);

		m_view.setCenter(340, GetPlayer().GetPosition().y - 213);


		m_frame.spriteFrame.setPosition(0, m_player.GetPosition().y - 470);
		m_frame.text.setPosition(20, m_player.GetPosition().y - 460);
		m_frame.text.setString("Очки: " + std::to_string(m_player.GetTotal()));
		m_frame.spriteButton.setPosition(550, m_player.GetPosition().y - 460);

	}
	else
	{
		m_menu.Animation(time);


	}
}

void CGame::Display(sf::RenderWindow & window)
{
	window.setView(m_view);
	window.clear(sf::Color::White);
	if (!m_menu.IsOpen())
	{


		GetLevel().Draw(window);

		DrawDonuts(window);
		DrawPuddles(window);
		DrawScrolls(window);



		for (auto elephant : m_elephants)
		{
			window.draw(elephant->GetHero());
			if (elephant->IsRun())
			{
				window.draw(elephant->GetBonus());
			}
		}



		for (auto cat : m_cats)
		{
			window.draw(cat->GetHero());
			if (cat->IsRun())
			{
				window.draw(cat->GetBonus());
			}
		}

		DrawEnemies(window);

		window.draw(GetPlayer().GetHero());

		if (GetPlayer().IsJump())
		{
			window.draw(m_player.GetSpriteProtection());
		}

		if (m_player.IsFreeze())
		{
			window.draw(m_player.GetSpriteFreeze());
		}


		if (m_player.IsFinalState())
		{
			window.draw(m_text);
			window.draw(m_frame.spriteButton);
		}


		window.draw(m_frame.spriteFrame);
		window.draw(m_frame.text);

	}
	else
	{
		window.draw(m_menu.GetSprite());
		window.draw(m_menu.GetSpriteAnimation());
		window.draw(m_menu.GetButton());
		if (sf::IntRect(570, 30, 91, 37).contains(sf::Mouse::getPosition(window)) &&
			(sf::Mouse::isButtonPressed(sf::Mouse::Left)))
		{
			m_menu.SetOpen();
			m_music.play();
		}
	}
	window.display();
}

CGame::~CGame()
{
}
