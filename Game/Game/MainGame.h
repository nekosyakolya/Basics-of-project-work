#pragma once
#include "MainMenu.h"

#include "level.h"

#include "Donut.h"

struct Frame
{
	sf::Texture textureFrame;
	sf::Sprite spriteFrame;


	sf::Texture textureButton;
	sf::Sprite spriteButton;
	sf::Text text;
};

class CMainGame
{
public:
	CMainGame();

	CMainMenu menu;
	Level level;

	CGame mission;
	sf::View view;


	CPlayer player;

	std::vector<std::shared_ptr<CDonut>> bonuses;
	std::vector<std::shared_ptr<CEnemy>> enemies;

	Object finish;
	sf::Font font;


	sf::Text text;

	sf::Music music;

	
	std::vector<std::shared_ptr<CCat>> cats;
	std::vector<std::shared_ptr<CPuddle>> puddles;
	std::vector<std::shared_ptr<CBlackCat>> blackCats;
	std::vector<std::shared_ptr<CMission>> missions;
	Frame frame;


	void Init();
	void Clear();


	sf::Clock GetClock();
	~CMainGame();


private:
	std::map <unsigned, std::string> m_levelMap;
	unsigned m_levelGame;
	sf::Clock m_clock;
	sf::Clock m_clockGame;

	Object hero;
	std::vector<Object> bonusStartPos;

	std::vector<Object> enemyStartPos;
	std::vector<Object> newMissionStart;

	std::vector<Object> puddleStartPos;
	std::vector<Object> catStartPos;

	std::vector<Object> blackCatStartPos;
};

