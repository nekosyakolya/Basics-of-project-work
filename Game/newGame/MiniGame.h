#pragma once
#include "level.h"

#include <map> 
#include <memory>


static const int SIZE_TILE = 58;
static const int WIDTH_PLAYER = 36;

enum struct PlayerDirection
{
	LEFT = -1,
	STAY = 0,
	RIGHT = 1,
	DOWN = 2,
	UP = 3
};



struct Player
{
	sf::Vector2f position;
	sf::Vector2f offset;
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
	float currentFrame;
	PlayerDirection direction;
	std::vector<Object> obj;
	Player() = default;

	void GetDirection();
	void Update(float time);
	void InitPlayer(Level &level, const sf::Vector2f &positionPlayer);
	sf::FloatRect GetRect();
	void CheckCollision(const float &dx, const float &dy);
	void CheckCentre(const float &dx);

};


struct Box
{
	sf::Vector2f position;
	bool isStateFinal;
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
	Box(const sf::Vector2f &position, bool isState);
};

class CMiniGame
{
public:
	CMiniGame();
	Level GetMap()const;
	Player m_player;
	std::vector<std::shared_ptr<Box>> m_boxes;
	void CheckCollision(float time);
	void Init();
	void SetString();
	sf::Text GetText()const;

	bool IsNext()const;

	sf::Sprite GetSprite()const;
	sf::Text GetTextMission()const;
	sf::Sprite GetSpriteMission()const;
	sf::Sprite GetRestart()const;

	void SetNext();
	void ChangeButton();

	void LevelUp();
	void Initialisation();

	unsigned GetTimePlay()const;
	unsigned GetTimeDelay()const;

	void SetTimeDelay();

	void SetText(const std::string &);
	void PlaySound();
	void StopSound();
	~CMiniGame();
private:
	Level m_map;
	Object m_hero;

	unsigned m_time;

	bool hasCollisionWithBox(const size_t &boxNo, const sf::Vector2f &offset);
	void CheckObjects(const size_t &boxNo, const sf::Vector2f &offset);
	void ChangePosition(Box &box, const sf::Vector2f &offset);

	void UpdateTime();
	sf::Clock m_gameTimeClock;
	sf::Texture m_texture;
	sf::Sprite m_sprite;

	sf::Text m_text;
	sf::Sprite m_spriteButton;
	sf::Texture m_textureButton;

	bool m_next;

	sf::Text m_textMission;
	sf::Font m_font;

	unsigned m_level;

	sf::Texture m_textureRestart;
	sf::Sprite m_spriteRestart;

	unsigned m_gameTime;

	sf::SoundBuffer m_buffer;
	sf::Sound m_sound;
};


