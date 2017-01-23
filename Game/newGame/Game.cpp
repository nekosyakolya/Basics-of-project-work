#include "stdafx.h"
#include "Game.h"


CGame::CGame()
{

}

void CGame::Initialisation()
{
	m_gameLevel = 1;
	m_font.loadFromFile("resources/CyrilicOld.TTF");

	m_view.reset(sf::FloatRect(0, 0, 680, 500));

	InitLevel();
	InitFrame();
	InitText();
	m_music.openFromFile("resources/main.wav");
	m_music.setVolume(10.5f);
	m_music.setLoop(true);
	m_miniGame.Init();

}



void CGame::UpdatePlayer(float time)
{
	m_player.Update(time);

}

void CGame::UpdateFrame()
{
	m_frame.spriteFrame.setPosition(0, m_player.GetPosition().y - 470);
	m_frame.text.setPosition(20, m_player.GetPosition().y - 460);
	m_frame.text.setString("Очки: " + std::to_string(m_player.GetTotal()));
	m_frame.spriteButton.setPosition(550, m_player.GetPosition().y - 460);
}

void CGame::UpdateEnemy(float time)
{
	for (auto enemy : m_enemies)
	{
		enemy->Update(time);
		CheckCollisionBetweenPlayerAndEnemy(time, &(*enemy));
		CheckCollisionBetweenFinishAndEnemy(&(*enemy));
		CheckCollisionBetweenBonusesAndEnemy(&(*enemy));
		CheckCollisionBetweenEnemies(&(*enemy));
		enemy->SetPosition();
	}
}

void CGame::UpdateGame(float time)
{
	UpdatePlayer(time);
	UpdateEnemy(time);

	CheckCollisionWithElephantes(time);
	CheckCollisionWithFinish();
	CheckCollisionWithScrolls();
	CheckCollisionWithCats(time);
	CheckCollisionWithTurtles(time);
	CheckCollisionWithDonuts();
	CheckCollisionWithPuddles();

	m_player.SetPosition();
	m_player.SetSpeed();
	m_player.Freezing();


	m_textResult.setPosition(180, m_player.GetPosition().y - 240);
	m_view.setCenter(340, m_player.GetPosition().y - 213);

	UpdateFrame();
}

void CGame::UpdateGameMini(float time)
{
	if (m_miniGame.IsNext())
	{
		if (m_miniGame.GetTimeDelay() != 10)
		{
			m_miniGame.SetString();

			m_miniGame.m_player.Update(time);

			m_miniGame.CheckCollision(time);
			m_miniGame.m_player.sprite.setPosition(m_miniGame.m_player.position);


			size_t finishedBoxesCount = 0;
			for (auto box : m_miniGame.m_boxes)
			{
				if (box->isStateFinal)
				{
					++finishedBoxesCount;
				}
			}

			if (finishedBoxesCount == m_miniGame.m_boxes.size() || m_miniGame.GetTimePlay() > 90)
			{
				m_miniGame.SetTimeDelay();
			}

		}
		else
		{
			std::string result = "";
			if (m_miniGame.GetTimePlay() > 90)
			{
				result = "Вы проиграли - ваша скорость упадет\n";
			}
			else
			{
				result = "Вы выиграли - ваша скорость вырастет\n";
			}
			m_miniGame.SetText(result + "Вперед,в игру!");
			m_miniGame.ChangeButton();
		}
	}
}

void CGame::InitFrame()
{
	m_frame.textureFrame.loadFromFile("resources/frame.png");
	m_frame.spriteFrame.setTexture(m_frame.textureFrame);
	m_frame.spriteFrame.setPosition(0, m_player.GetPosition().y - 470);

	m_frame.textureButton.loadFromFile("resources/frame-next.png");
	m_frame.spriteButton.setTexture(m_frame.textureButton);
	m_frame.spriteButton.setPosition(440, m_player.GetPosition().y - 460);

	m_frame.text.setString("");
	m_frame.text.setFont(m_font);
	m_frame.text.setCharacterSize(24);
}

void CGame::InitText()
{

	m_textResult.setCharacterSize(32);
	m_textResult.setString("");
	m_textResult.setFont(m_font);
	m_textResult.setFillColor(sf::Color::White);
	m_textResult.setStyle(sf::Text::Bold);
	m_textResult.setPosition(20, m_player.GetPosition().y - 460);
}

void CGame::InitPlayer()
{

	Object hero = m_level.GetObject("player");
	m_player.Init(m_level);
	m_player.InitPlayer(sf::Vector2f(hero.rect.left, hero.rect.top));

}

void CGame::InitBonuses()
{
	std::vector<Object> bonusStartPos = m_level.GetObjects("bonus");
	for (auto currBonus : bonusStartPos)
	{
		m_bonuses.push_back(std::shared_ptr<CDonut>(new CDonut(sf::Vector2f(currBonus.rect.left, currBonus.rect.top))));
	}

	std::vector<Object> puddleStartPos = m_level.GetObjects("puddle");
	for (auto currPuddle : puddleStartPos)
	{
		m_puddles.push_back(std::shared_ptr<CPuddle>(new CPuddle(sf::Vector2f(currPuddle.rect.left, currPuddle.rect.top))));
	}


	std::vector<Object> newMissionStart = m_level.GetObjects("newMission");
	for (auto currBonus : newMissionStart)
	{
		m_scrolls.push_back(std::shared_ptr<CScroll>(new CScroll(sf::Vector2f(currBonus.rect.left, currBonus.rect.top))));
	}
}

void CGame::InitEnemies()
{
	std::vector<Object> enemyStartPos = m_level.GetObjects("enemy");
	sf::Clock clock;
	for (auto currEnemy : enemyStartPos)
	{
		float timeSpeed = static_cast<float>(clock.getElapsedTime().asMicroseconds());
		clock.restart();
		timeSpeed = timeSpeed / 10000000;
		m_enemies.push_back(std::shared_ptr<CEnemy>(new CEnemy(sf::Vector2f(currEnemy.rect.left, currEnemy.rect.top), timeSpeed, m_enemyMap.find(m_gameLevel)->second)));
	}
	for (auto enemy : m_enemies)
	{
		enemy->Init(m_level);
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


	std::vector<Object> turtleStartPos = m_level.GetObjects("turtle");
	for (auto currTurtle : turtleStartPos)
	{
		m_turtles.push_back(std::shared_ptr<CTurtle>(new CTurtle(sf::Vector2f(currTurtle.rect.left, currTurtle.rect.top))));
	}
	for (auto turtle : m_turtles)
	{
		turtle->Init(m_level);
	}

}

void CGame::InitLevel()
{
	m_level.LoadFromFile(m_levelMap.find(m_gameLevel)->second);
	InitPlayer();
	InitBonuses();
	InitEnemies();
	m_finish = m_level.GetObject("finish");
}

void CGame::ClearLevel()
{
	m_bonuses.clear();
	m_enemies.clear();
	m_scrolls.clear();
	m_puddles.clear();
	m_cats.clear();
	m_elephants.clear();
	m_turtles.clear();
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

void CGame::CheckCollisionWithScrolls()
{
	for (size_t i = 0; i < m_scrolls.size(); ++i)
	{
		if (m_player.GetRect().intersects(m_scrolls[i]->GetSprite().getGlobalBounds()))
		{
			m_scrolls.erase(m_scrolls.begin() + i);
			m_player.SetNewMission();
			m_miniGame.PlaySound();
		}
	}
}

void CGame::CheckCollisionWithFinish()
{
	if (m_player.GetRect().intersects(m_finish.rect) && !m_player.IsFinalState())
	{
		m_player.SetFinalState();
		m_player.SetPlaceInFinal();
		m_player.UpdateTotal(210 * 1 / m_player.GetPlaceInFinal());
		m_textResult.setString("Вы заняли " + std::to_string(m_player.GetPlaceInFinal()) + " место\n");
	}
}

void CGame::CheckCollisionWithElephantes(float time)
{
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
}

void CGame::CheckCollisionWithCats(float time)
{
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
}


void CGame::CheckCollisionWithTurtles(float time)
{
	for (auto turtle : m_turtles)
	{
		if (!(turtle->IsStop(m_player.GetPosition().y)))
		{
			turtle->Update(time);
			turtle->SetPosition();
		}

		if (m_player.GetRect().intersects(turtle->GetRectBonus()) && turtle->IsShow())
		{
			turtle->SetShow();
			m_player.UpdateTotal(-m_player.GetTotal());

		}

	}
}


void CGame::CheckCollisionBetweenPlayerAndEnemy(float time, CEnemy *enemy)
{
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
}

void CGame::CheckCollisionBetweenFinishAndEnemy(CEnemy * enemy)
{
	if (m_finish.rect.intersects(enemy->GetHero().getGlobalBounds()) && !enemy->IsFinalState())
	{
		enemy->SetFinalState();
		m_player.SetPlaceInFinal();

	}
}

void CGame::CheckCollisionBetweenBonusesAndEnemy(CEnemy * enemy)
{
	for (size_t i = 0; i < m_bonuses.size(); ++i)
	{
		if (enemy->GetRect().intersects(m_bonuses[i]->GetSprite().getGlobalBounds()))
		{
			m_bonuses.erase(m_bonuses.begin() + i);
		}
	}

	for (auto scroll : m_scrolls)
	{
		if (enemy->GetRect().intersects(scroll->GetSprite().getGlobalBounds()))
		{
			enemy->SetProtection();
		}
	}

}

void CGame::CheckCollisionBetweenEnemies(CEnemy * enemy)
{
	for (auto elephant : m_elephants)
	{
		if ((enemy->GetRect().intersects(elephant->GetRectBonus()) && elephant->IsShow())
			|| enemy->GetRect().intersects(elephant->GetRect()))
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


	for (auto turtle : m_turtles)
	{
		if ((enemy->GetRect().intersects(turtle->GetRectBonus()) && turtle->IsShow()) || enemy->GetRect().intersects(turtle->GetRect()))
		{
			enemy->SetProtection();
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

void CGame::DrawElephants(sf::RenderWindow &window) const
{
	for (auto elephant : m_elephants)
	{
		window.draw(elephant->GetHero());
		if (elephant->IsRun())
		{
			window.draw(elephant->GetBonus());
		}
	}
}

void CGame::DrawCats(sf::RenderWindow &window) const
{
	for (auto cat : m_cats)
	{
		window.draw(cat->GetHero());
		if (cat->IsRun())
		{
			window.draw(cat->GetBonus());
		}
	}


	for (auto turtle : m_turtles)
	{
		window.draw(turtle->GetHero());
		if (turtle->IsRun())
		{
			window.draw(turtle->GetBonus());
		}
	}
}

void CGame::DrawPlayer(sf::RenderWindow &window)
{
	window.draw(m_player.GetHero());

	if (m_player.IsJump())
	{
		window.draw(m_player.GetSpriteProtection());
	}

	if (m_player.IsFreeze())
	{
		window.draw(m_player.GetSpriteFreeze());
	}


	if (m_player.IsFinalState())
	{
		window.draw(m_textResult);
		window.draw(m_frame.spriteButton);

		if (sf::FloatRect(550, 15, 60, 60).contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))) &&
			(sf::Mouse::isButtonPressed(sf::Mouse::Left)))
		{
			++m_gameLevel;
			ClearLevel();
			InitLevel();

		}
	}

}

void CGame::DrawEndOfMiniGame(sf::RenderWindow &window)
{
	m_view.setCenter(window.getSize().x / 2.0f + 5, window.getSize().y / 2.0f);
	window.setView(m_view);

	window.clear(sf::Color::White);

	window.draw(m_miniGame.GetSprite());

	window.draw(m_miniGame.GetTextMission());

	window.draw(m_miniGame.GetSpriteMission());

	if (sf::IntRect(270, 330, 151, 61).contains(sf::Mouse::getPosition(window)) &&
		(sf::Mouse::isButtonPressed(sf::Mouse::Left)))
	{
		m_player.SetNewMission();

		if (m_miniGame.GetTimePlay() > 90)
		{
			m_player.ChangePosition();
			m_player.UpdateTotal(-5);
		}
		else
		{
			m_player.UpdateDelta();
			m_player.UpdateTotal(50);
		}
		m_miniGame.LevelUp();
		m_miniGame.Initialisation();
		m_miniGame.StopSound();
		m_music.play();
	}
}

void CGame::DrawMenu(sf::RenderWindow &window)
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

void CGame::DrawMenuMini(sf::RenderWindow &window)
{
	m_view.setCenter(window.getSize().x / 2.0f + 5, window.getSize().y / 2.0f);
	window.setView(m_view);

	window.clear(sf::Color::White);

	window.draw(m_miniGame.GetSprite());

	window.draw(m_miniGame.GetTextMission());

	window.draw(m_miniGame.GetSpriteMission());

	if (sf::IntRect(270, 330, 151, 61).contains(sf::Mouse::getPosition(window)) &&
		(sf::Mouse::isButtonPressed(sf::Mouse::Left)))
	{
		m_miniGame.SetNext();
	}
}

void CGame::DrawGame(sf::RenderWindow &window)
{
	if (!m_player.IsNewMission())
	{
		m_level.Draw(window);
		DrawDonuts(window);
		DrawPuddles(window);
		DrawScrolls(window);
		DrawElephants(window);
		DrawCats(window);
		DrawEnemies(window);
		window.draw(m_frame.spriteFrame);
		window.draw(m_frame.text);
		DrawPlayer(window);
	}
	else
	{
		DrawGameMini(window);
	}
}

void CGame::DrawGameMini(sf::RenderWindow &window)
{
	if (m_miniGame.IsNext())
	{
		if (m_miniGame.GetTimeDelay() != 10)
		{
			m_view.setCenter(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
			window.setView(m_view);
			window.clear(sf::Color::White);
			m_miniGame.GetMap().Draw(window);
			for (auto box : m_miniGame.m_boxes)
			{
				window.draw(box->sprite);
			}
			window.draw(m_miniGame.GetText());
			window.draw(m_miniGame.m_player.sprite);
			window.draw(m_miniGame.GetRestart());

			if (sf::IntRect(10, 10, 91, 37).contains(sf::Mouse::getPosition(window)) &&
				(sf::Mouse::isButtonPressed(sf::Mouse::Left)))
			{
				m_miniGame.Initialisation();
			}
		}
		else
		{
			DrawEndOfMiniGame(window);
		}
	}
	else
	{
		DrawMenuMini(window);
	}
}

void CGame::Update(float time)
{

	if (!m_menu.IsOpen() && !m_player.IsNewMission())
	{
		UpdateGame(time);
	}
	else if (m_player.IsNewMission())
	{
		m_music.pause();
		UpdateGameMini(time);
	}
	else if (m_menu.IsOpen())
	{
		m_menu.Animation(time);
	}
}

void CGame::Display(sf::RenderWindow & window)
{
	window.setView(m_view);
	window.clear(sf::Color::White);
	(!m_menu.IsOpen()) ? DrawGame(window) : DrawMenu(window);
	window.display();
}

CGame::~CGame()
{
}
