#include "stdafx.h"
#include "MainGame.h"


CMainGame::CMainGame()
{
	m_levelGame = 1;

	m_levelMap = { { 1, "resources/map.tmx" },{ 2, "resources/map2.tmx" } };

	view.reset(sf::FloatRect(0, 0, 680, 500));
	font.loadFromFile("resources/CyrilicOld.TTF");

	text.setString("");
	text.setFont(font);
	text.setCharacterSize(32);
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);
	text.setPosition(40, 20);


	music.openFromFile("resources/main.wav");
	music.setVolume(10.5f);
	music.setLoop(true);

	frame.textureFrame.loadFromFile("resources/frame.png");
	frame.spriteFrame.setTexture(frame.textureFrame);
	frame.spriteFrame.setPosition(0, player.GetPosition().y - 470);

	frame.textureButton.loadFromFile("resources/frame-next.png");
	frame.spriteButton.setTexture(frame.textureButton);
	frame.spriteButton.setPosition(440, player.GetPosition().y - 460);

	frame.text.setString("");
	frame.text.setFont(font);
	frame.text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);
	text.setPosition(20, player.GetPosition().y - 460);

	Init();
}


void CMainGame::Init()
{

	level.LoadFromFile(m_levelMap.find(m_levelGame)->second);
	hero = level.GetObject("player");

	player.Initialisation(sf::Vector2f(hero.rect.left, hero.rect.top));
	player.Init(level);

	mission.Init();

	bonusStartPos = level.GetObjects("bonus");


	for (auto currBonus : bonusStartPos)
	{
		bonuses.push_back(std::shared_ptr<CDonut>(new CDonut(sf::Vector2f(currBonus.rect.left, currBonus.rect.top))));
	}

	enemyStartPos = level.GetObjects("enemy");

	for (auto currEnemy : enemyStartPos)
	{
		float timeSpeed = static_cast<float>(m_clock.getElapsedTime().asMicroseconds());
		m_clock.restart();
		timeSpeed = timeSpeed / 10000000;
		enemies.push_back(std::shared_ptr<CEnemy>(new CEnemy(sf::Vector2f(currEnemy.rect.left, currEnemy.rect.top), timeSpeed)));
	}


	for (auto enemy : enemies)
	{
		enemy->Init(level);
	}

	finish = level.GetObject("finish");

	newMissionStart = level.GetObjects("newMission");


	for (auto currBonus : newMissionStart)
	{
		missions.push_back(std::shared_ptr<CMission>(new CMission(sf::Vector2f(currBonus.rect.left, currBonus.rect.top))));
	}

	
	puddleStartPos = level.GetObjects("puddle");
	for (auto currPuddle : puddleStartPos)
	{
		puddles.push_back(std::shared_ptr<CPuddle>(new CPuddle(sf::Vector2f(currPuddle.rect.left, currPuddle.rect.top))));
	}

	catStartPos = level.GetObjects("cat");

	for (auto currCat : catStartPos)
	{
		cats.push_back(std::shared_ptr<CCat>(new CCat(sf::Vector2f(currCat.rect.left, currCat.rect.top))));
	}

	for (auto cat : cats)
	{
		cat->Init(level);
	}

	blackCatStartPos = level.GetObjects("blackCat");

	for (auto currCat : blackCatStartPos)
	{
		blackCats.push_back(std::shared_ptr<CBlackCat>(new CBlackCat(sf::Vector2f(currCat.rect.left, currCat.rect.top))));
	}

	for (auto cat : blackCats)
	{
		cat->Init(level);
	}
}

void CMainGame::Clear()
{
	bonuses.clear();
	enemies.clear();
	missions.clear();
	puddles.clear();
	cats.clear();
	blackCats.clear();
}

sf::Clock CMainGame::GetClock()
{
	return m_clockGame;
}

CMainGame::~CMainGame()
{
}
