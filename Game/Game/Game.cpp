#include "stdafx.h"



using namespace sf;
using namespace std;

struct Frame
{
	sf::Texture textureFrame;
	sf::Sprite spriteFrame;
	sf::Text text;
};

void HandleEvents(RenderWindow &window)
{
	Event event;
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed ||
			(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
		{
			window.close();
		}
	}
}



void UpdateView(float x, float y, View &view)
{
	view.setCenter(x + 20, y - 213);
}

void CheckCollision(std::vector<CDonut*> &bonuses, CPlayer & player)
{
	
	for (size_t i = 0; i < bonuses.size(); ++i)
	{
		if (player.GetRect().intersects(bonuses[i]->GetSprite().getGlobalBounds()))
		{
			delete bonuses[i];
			bonuses.erase(bonuses.begin() + i);
			player.PlaySound();
			player.ChangePosition();
			player.UpdateTotal(5);
		}
	}
}

void Display(RenderWindow &window, CPlayer &player, View &view, Level & level, std::vector<CDonut*> &bonuses, std::vector<CEnemy*> &enemies, Text &text, std::vector<CMission*> &missions, std::vector<CPuddle*> &puddles, std::vector<CCat*> &cats, Frame &frame, std::vector<CBlackCat*> &blackCats)
{
	window.setView(view);
	window.clear(Color::White);
	level.Draw(window);

	for (auto bonus : bonuses)
	{
		window.draw(bonus->GetSprite());
	}

	for (auto mission : missions)
	{
		window.draw(mission->GetSprite());
	}

	for (auto puddle : puddles)
	{
		window.draw(puddle->GetSprite());
	}


	for (auto cat : cats)
	{
		window.draw(cat->GetHero());
		if (cat->IsRun())
		{
			window.draw(cat->GetBonus());
		}
	}
	for (auto cat : blackCats)
	{
		window.draw(cat->GetHero());
		if (cat->IsRun())
		{
			window.draw(cat->GetBonus());
		}
	}


	for (auto enemy : enemies)
	{
		window.draw(enemy->GetHero());

		if (enemy->IsJump())
		{
			window.draw(enemy->GetRectProtection());
		}
	}

	window.draw(player.GetHero());
	if (player.IsFinalState())
	{
		window.draw(text);
	}

	if (player.IsJump())
	{
		window.draw(player.GetSpriteProtection());
	}

	if (player.IsFreeze())
	{
		window.draw(player.GetSpriteFreeze());
	}

	window.draw(frame.spriteFrame);
	window.draw(frame.text);

	window.display();
}

void Update(CPlayer &player, View &view, std::vector<CDonut*> &bonuses, std::vector<CEnemy*> &enemies, Object &finish, Text &text, std::vector<CMission*> &missions, float time, std::vector<CPuddle*> &puddles, std::vector<CCat*> &cats, CGame &mission, std::vector<CBlackCat*> &blackCats)
{
	player.Update(time);
	for (auto enemy : enemies)
	{
		enemy->Update(time);

		if (player.GetRect().intersects(enemy->GetHero().getGlobalBounds()))
		{
			if (player.GetOffset().x < 0)
			{
				player.UpdatePosition(enemy->GetPosition().x + 40);
				enemy->UpdatePosition(-0.1f * time);
			}
			if (player.GetOffset().x > 0)
			{
				player.UpdatePosition(enemy->GetPosition().x - 40);
				enemy->UpdatePosition(0.1f * time);
			}


			if (player.GetOffset().x == 0)
			{
				if (!enemy->Check(0.1f * time))
				{
					enemy->Check(-0.1f * time);
				}
				
			}
		}

		if (finish.rect.intersects(enemy->GetHero().getGlobalBounds()) && !enemy->IsFinalState())
		{
			enemy->SetFinalState();
			player.SetPlaceInFinal();

		}


		for (size_t i = 0; i < bonuses.size(); ++i)
		{
			if (enemy->GetRect().intersects(bonuses[i]->GetSprite().getGlobalBounds()))
			{
				delete bonuses[i];
				bonuses.erase(bonuses.begin() + i);
			}
		}


		for (auto cat : cats)
		{
			if ((enemy->GetRect().intersects(cat->GetRectBonus()) && cat->IsShow()) || enemy->GetRect().intersects(cat->GetRect()))
			{
				enemy->SetProtection();
			}
		}


		for (auto cat : blackCats)
		{
			if ((enemy->GetRect().intersects(cat->GetRectBonus()) && cat->IsShow()) || enemy->GetRect().intersects(cat->GetRect()))
			{
				enemy->SetProtection();
			}

		}


		for (auto scroll : missions)
		{
			if (enemy->GetRect().intersects(scroll->GetSprite().getGlobalBounds()))
			{
				enemy->SetProtection();
			}
		}

		enemy->SetPosition();
	}



	if (player.GetRect().intersects(finish.rect) && !player.IsFinalState())
	{
		player.SetFinalState();
		player.SetPlaceInFinal();
		player.UpdateTotal(210 * 1 / player.GetPlaceInFinal());
		text.setString("Вы заняли " + to_string(player.GetPlaceInFinal()) + " место\n");
	}

	
	for (size_t i = 0; i < missions.size(); ++i)
	{
		if (player.GetRect().intersects(missions[i]->GetSprite().getGlobalBounds()))
		{
			delete missions[i];
			missions.erase(missions.begin() + i);
			player.SetNewMission();
			mission.PlaySound();
		}
	}

	for (auto puddle : puddles)
	{
		if (player.GetRect().intersects(puddle->GetSprite().getGlobalBounds()) && 
			!player.IsJump() && !puddle->IsVisited())
		{
			//уменьшаем скорость
			//звук воды
			//скорость ненадолго уменьшаем
			player.ChangePosition();
			player.UpdateTotal(-5);
			puddle->PlaySound();
			puddle->SetVisited();
		}
	}



	CheckCollision(bonuses, player);
	player.SetPosition();


	for (auto cat : cats)
	{
		if (!(cat->IsStop(player.GetPosition().y)))
		{
			cat->Update(time);
			cat->SetPosition();
		}

		if (player.GetRect().intersects(cat->GetRectBonus()) && cat->IsShow())
		{
			cat->SetShow();
			player.InitClock();
			player.SetDelay();
			player.UpdateTotal(100);
		}

	}


	for (auto cat : blackCats)
	{
		if (!(cat->IsStop(player.GetPosition().y)))
		{
			cat->Update(time);
			cat->SetPosition();
		}

		if (player.GetRect().intersects(cat->GetRectBonus()) && cat->IsShow())
		{
			cat->SetShow();
			player.InitClock();
			player.SetFreeze();
		}

	}
	player.SetSpeed();
	player.Freezing();

	text.setPosition(180, player.GetPosition().y - 240);
	UpdateView(320, player.GetPosition().y, view);

}


void ChangePosition(Box &box, Player & player, const sf::Vector2f &offset)
{
	player.position.x -= player.offset.x;
	player.position.y -= player.offset.y;
	player.direction = PlayerDirection::STAY;
	box.sprite.setPosition(box.sprite.getPosition().x + (-offset.x), box.sprite.getPosition().y + (-offset.y));
}

// TODO: pass reference to Box instead of boxNo
bool hasCollisionWithBox(std::vector<std::shared_ptr<Box>> &boxes, Player & player, const size_t &boxNo, const sf::Vector2f &offset)
{
	bool isCollision = false;
	boxes[boxNo]->sprite.setPosition(boxes[boxNo]->sprite.getPosition().x + offset.x, boxes[boxNo]->sprite.getPosition().y + offset.y);
	for (size_t j = 0; j < boxes.size(); ++j)
	{
		auto &box1 = boxes[boxNo];
		auto &box2 = boxes[j];
		if (boxes[boxNo]->sprite.getPosition() == boxes[j]->sprite.getPosition() && (&box1 != &box2))
		{
			ChangePosition(*(boxes[boxNo]), player, offset);
			isCollision = true;
		}
	}
	return isCollision;
}



// TODO: pass reference to Box instead of boxNo
void CheckObjects(std::vector<std::shared_ptr<Box>> &boxes, Player & player, const size_t &boxNo, const sf::Vector2f &offset)
{
	bool isCollision = hasCollisionWithBox(boxes, player, boxNo, offset);

	for (size_t j = 0; j < player.obj.size(); ++j)
	{
		auto &solidObject = player.obj[j];

		if ((solidObject.rect.intersects((*boxes[boxNo]).sprite.getGlobalBounds())) && (solidObject.name == "collision"))
		{
			ChangePosition(*(boxes[boxNo]), player, offset);
			isCollision = true;
			break;
		}

		if ((solidObject.rect.intersects((*boxes[boxNo]).sprite.getGlobalBounds())) && (solidObject.name == "goal"))
		{

			if (!boxes[boxNo]->isStateFinal)
			{
				boxes[boxNo]->isStateFinal = true;
			}

			isCollision = true;
		}
	}
	if (!isCollision && boxes[boxNo]->isStateFinal)
	{
		boxes[boxNo]->isStateFinal = false;
	}
}

void CheckCollisionMini(std::vector<std::shared_ptr<Box>> &boxes, Player & player, float time)
{
	sf::Vector2f offset;
	for (size_t i = 0; i < boxes.size(); ++i)
	{
		if (player.GetRect().intersects(boxes[i]->sprite.getGlobalBounds()))
		{
			player.offset.x *= time;
			player.offset.y *= time;
			if (player.offset.x < 0)
			{
				offset.x = -SIZE_TILE;
				offset.y = 0;
			}
			if (player.offset.x > 0)
			{
				offset.x = SIZE_TILE;
				offset.y = 0;
			}
			if (player.offset.y > 0)
			{

				offset.x = 0;
				offset.y = SIZE_TILE;
			}

			if (player.offset.y < 0)
			{
				offset.x = 0;
				offset.y = -SIZE_TILE;
			}
			CheckObjects(boxes, player, i, offset);
		}


		auto positionSprite = (boxes[i]->isStateFinal) ? Vector2f(0, 116) : Vector2f(0, 58);

		boxes[i]->sprite.setTextureRect(IntRect(static_cast<int>(positionSprite.x), static_cast<int>(positionSprite.y), 58, 58));

	}
}

void EnterGameLoop(RenderWindow &window, CPlayer &player, View &view, Level & level, Clock & clock, std::vector<CDonut*> &bonuses, std::vector<CEnemy*> &enemies, Object &finish, Text &text, std::vector<CMission*> &missions, CGame &mission, CMainMenu &menu, Music &music, std::vector<CPuddle*> &puddles, std::vector<CCat*> &cats, Frame &frame, std::vector<CBlackCat*> &blackCats)
{
	while (window.isOpen())
	{
		float time = static_cast<float>(clock.getElapsedTime().asMicroseconds());
		clock.restart();
		time = time / 800;

		HandleEvents(window);
		if (!menu.IsOpen())
		{
			if (!player.IsNewMission())
			{
				
				Update(player, view, bonuses, enemies, finish, text, missions, time, puddles, cats, mission, blackCats);

				frame.spriteFrame.setPosition(0, player.GetPosition().y - 470);
				frame.text.setPosition(20, player.GetPosition().y - 460);
				frame.text.setString("Очки: " + to_string(player.GetTotal()));
				Display(window, player, view, level, bonuses, enemies, text, missions, puddles, cats, frame, blackCats);
			}
			else
			{
				music.pause();
				if (mission.IsNext())
				{

					if (mission.GetTimeDelay() != 10)
					{

						mission.SetString();

						mission.m_player.Update(time);


						CheckCollisionMini(mission.m_boxes, mission.m_player, time);
						mission.m_player.sprite.setPosition(mission.m_player.position);



						view.setCenter(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
						window.setView(view);

						window.clear(Color::White);



						mission.GetMap().Draw(window);
						size_t finishedBoxesCount = 0;
						for (auto box : mission.m_boxes)
						{
							window.draw(box->sprite);
							if (box->isStateFinal)
							{
								++finishedBoxesCount;
							}
						}


						if (finishedBoxesCount == mission.m_boxes.size() || mission.GetTimePlay() > 90)
						{
							mission.SetTimeDelay();


						}

						window.draw(mission.GetText());
						window.draw(mission.m_player.sprite);

						window.draw(mission.GetRestart());

						if (IntRect(10, 10, 91, 37).contains(Mouse::getPosition(window)) &&
							(Mouse::isButtonPressed(Mouse::Left)))
						{
							mission.Initialisation();
						}

						window.display();
					}
					else
					{
						std::string result = "";
						if (mission.GetTimePlay() > 90)
						{
							result = "Вы проиграли - ваша скорость упадет\n";
						}
						else
						{
							result = "Вы выиграли - ваша скорость вырастет\n";
						}
						mission.SetText(result + "Вперед,в игру!");
						mission.ChangeButton();


						view.setCenter(window.getSize().x / 2.0f + 5, window.getSize().y / 2.0f);
						window.setView(view);

						window.clear(Color::White);

						window.draw(mission.GetSprite());

						window.draw(mission.GetTextMission());

						window.draw(mission.GetSpriteMission());


						window.display();

						if (IntRect(270, 330, 151, 61).contains(Mouse::getPosition(window)) &&
							(Mouse::isButtonPressed(Mouse::Left)))
						{
							player.SetNewMission();

							if (mission.GetTimePlay() > 90)
							{
								player.ChangePosition();
								player.UpdateTotal(-5);
							}
							else
							{
								player.UpdateDelta();
								player.UpdateTotal(50);
							}
							mission.LevelUp();
							mission.Initialisation();
							mission.StopSound();
							music.play();
						}

					}


					
				}
				else
				{

					view.setCenter(window.getSize().x / 2.0f + 5, window.getSize().y / 2.0f);
					window.setView(view);

					window.clear(Color::White);

					window.draw(mission.GetSprite());

					window.draw(mission.GetTextMission());

					window.draw(mission.GetSpriteMission());


					window.display();

					if (IntRect(270, 330, 151, 61).contains(Mouse::getPosition(window)) &&
						(Mouse::isButtonPressed(Mouse::Left)))
					{
						mission.SetNext();
					}


				}

			}
		}
		else
		{

			menu.Animation(time);

			window.setView(view);
			window.clear(Color::White);
			window.draw(menu.GetSprite());
			window.draw(menu.GetSpriteAnimation());
			window.draw(menu.GetButton());

			window.display();

			if (IntRect(570, 30, 91, 37).contains(Mouse::getPosition(window)) &&
				(Mouse::isButtonPressed(Mouse::Left)))
			{
				menu.SetOpen();
				music.play();
			}
		}
	}
}




int main()
{
	Level level;
	if (!level.LoadFromFile("resources/map.tmx"))
	{
		return EXIT_FAILURE;
	}

	CGame mission;

	mission.Init();

	Clock clock;
	ContextSettings settings;
	sf::View view;

	RenderWindow window(VideoMode(680, 500), "Game", Style::Close, settings);
	Vector2f windowCenter = Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
	view.reset(sf::FloatRect(0, 0, 680, 500));

	
	Object hero = level.GetObject("player");
	CPlayer player(Vector2f(hero.rect.left, hero.rect.top));
	player.Init(level);

	std::vector<Object> bonusStartPos = level.GetObjects("bonus");
	std::vector<CDonut*> bonuses;

	for (auto currBonus : bonusStartPos)
	{
		bonuses.push_back(new CDonut(Vector2f(currBonus.rect.left, currBonus.rect.top)));
	}

	std::vector<Object> enemyStartPos = level.GetObjects("enemy");
	std::vector<CEnemy*> enemies;

	for (auto currEnemy : enemyStartPos)
	{
		float timeSpeed = static_cast<float>(clock.getElapsedTime().asMicroseconds());
		clock.restart();
		timeSpeed = timeSpeed / 10000000;
		enemies.push_back(new CEnemy(Vector2f(currEnemy.rect.left, currEnemy.rect.top), timeSpeed));
	}

	

	for (auto enemy : enemies)
	{
		enemy->Init(level);
	}

	Object finish = level.GetObject("finish");


	std::vector<Object> newMissionStart = level.GetObjects("newMission");
	std::vector<CMission*> missions;

	for (auto currBonus : newMissionStart)
	{
		missions.push_back(new CMission(Vector2f(currBonus.rect.left, currBonus.rect.top)));
	}



	Font font;
	font.loadFromFile("resources/CyrilicOld.TTF");
	Text text("", font, 32);
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);
	text.setPosition(40, 20);


	CMainMenu menu;


	Music music;
	music.openFromFile("resources/main.wav");
	music.setVolume(10.5f);
	music.setLoop(true);


	std::vector<Object> puddleStartPos = level.GetObjects("puddle");
	std::vector<CPuddle*> puddles;

	for (auto currPuddle : puddleStartPos)
	{
		puddles.push_back(new CPuddle(Vector2f(currPuddle.rect.left, currPuddle.rect.top)));
	}



	std::vector<Object> catStartPos = level.GetObjects("cat");
	std::vector<CCat*> cats;

	for (auto currCat : catStartPos)
	{
		cats.push_back(new CCat(Vector2f(currCat.rect.left, currCat.rect.top)));
	}

	for (auto cat : cats)
	{
		cat->Init(level);
	}



	std::vector<Object> blackCatStartPos = level.GetObjects("blackCat");
	std::vector<CBlackCat*> blackCats;

	for (auto currCat : blackCatStartPos)
	{
		blackCats.push_back(new CBlackCat(Vector2f(currCat.rect.left, currCat.rect.top)));
	}

	for (auto cat : blackCats)
	{
		cat->Init(level);
	}


	Frame frame;

	frame.textureFrame.loadFromFile("resources/frame.png");
	frame.spriteFrame.setTexture(frame.textureFrame);
	frame.spriteFrame.setPosition(0, player.GetPosition().y - 470);
	frame.text.setString("");
	frame.text.setFont(font);
	frame.text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);
	text.setPosition(20, player.GetPosition().y - 460);


	EnterGameLoop(window, player, view, level, clock, bonuses, enemies, finish, text, missions, mission, menu, music, puddles, cats, frame, blackCats);

	return EXIT_SUCCESS;
}
