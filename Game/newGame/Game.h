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

	Level GetLevel()const;
	CPlayer GetPlayer()const;


	void Update(float);

	void Display(sf::RenderWindow &);

	~CGame();
private:
	Level m_level;
	CPlayer m_player;
	CMainMenu m_menu;
	sf::View m_view;
	sf::Text m_text;
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



	void CheckCollisionWithDonuts();
	void CheckCollisionWithPuddles();

	void UpdatePlayer(float);

	void DrawDonuts(sf::RenderWindow &)const;
	void DrawEnemies(sf::RenderWindow &)const;
	void DrawPuddles(sf::RenderWindow &)const;
	void DrawScrolls(sf::RenderWindow &)const;


};

