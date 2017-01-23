#pragma once

#include "level.h"
#include "Player.h"
#include "Donut.h"
#include "Enemy.h"
#include "Puddle.h"
#include "Scroll.h"
#include "Elephant.h"
#include "Cat.h"
#include "MainMenu.h"
#include "MiniGame.h"



#include <memory>

struct Frame
{
	sf::Texture textureFrame;
	sf::Sprite spriteFrame;
	sf::Texture textureButton;
	sf::Sprite spriteButton;
	sf::Text text;
};

class CGame
{
public:
	CGame();
	void Initialisation();
	void Update(float);
	void Display(sf::RenderWindow &);

	~CGame();
private:
	Level m_level;
	CPlayer m_player;
	CMainMenu m_menu;
	CMiniGame m_miniGame;

	sf::View m_view;
	sf::Text m_textResult;
	sf::Font m_font;
	sf::Music m_music;
	Frame m_frame;

	Object m_finish;
	std::vector<std::shared_ptr<CDonut>> m_bonuses;
	std::vector<std::shared_ptr<CEnemy>> m_enemies;
	std::vector<std::shared_ptr<CPuddle>> m_puddles;
	std::vector<std::shared_ptr<CScroll>> m_scrolls;
	std::vector<std::shared_ptr<CElephant>> m_elephants;
	std::vector<std::shared_ptr<CCat>> m_cats;

	void InitFrame();
	void InitText();
	void InitPlayer();
	void InitBonuses();
	void InitEnemies();
	void InitLevel();
	void ClearLevel();

	void CheckCollisionWithDonuts();
	void CheckCollisionWithPuddles();
	void CheckCollisionWithScrolls();
	void CheckCollisionWithFinish();
	void CheckCollisionWithElephantes(float time);
	void CheckCollisionWithCats(float time);
	void CheckCollisionBetweenPlayerAndEnemy(float time, CEnemy *enemy);
	void CheckCollisionBetweenFinishAndEnemy(CEnemy *enemy);
	void CheckCollisionBetweenBonusesAndEnemy(CEnemy *enemy);
	void CheckCollisionBetweenEnemies(CEnemy *enemy);

	void UpdatePlayer(float);
	void UpdateFrame();
	void UpdateEnemy(float);
	void UpdateGame(float);
	void UpdateGameMini(float);

	void DrawDonuts(sf::RenderWindow &)const;
	void DrawEnemies(sf::RenderWindow &)const;
	void DrawPuddles(sf::RenderWindow &)const;
	void DrawScrolls(sf::RenderWindow &)const;
	void DrawElephants(sf::RenderWindow &)const;
	void DrawCats(sf::RenderWindow &)const;
	void DrawPlayer(sf::RenderWindow &);

	void DrawMenu(sf::RenderWindow &);
	void DrawMenuMini(sf::RenderWindow &);

	void DrawGame(sf::RenderWindow &);
	void DrawGameMini(sf::RenderWindow &);
	unsigned m_gameLevel;

	std::map <unsigned, std::string> m_levelMap = { { 1, "resources/map.tmx" },{ 2, "resources/map2.tmx" } };

};

