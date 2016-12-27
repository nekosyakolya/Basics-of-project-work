#pragma once
#include <SFML/Graphics.hpp>
#include "level.h"

enum struct Direction
{
	LEFT = -1,
	UP = 0,
	RIGHT = 1
};

class CPlayer
{
public:
	CPlayer(sf::Vector2f);
	sf::RectangleShape GetPlayer() const;
	sf::Vector2f GetPosition()const;
	void GetDirection();
	void Update(float time);
	void CheckCollision(float dx, float dy);
	void InitPlayer(Level &level);
	~CPlayer();
private:
	sf::RectangleShape m_object;
	sf::Vector2f m_position;
	sf::Vector2f m_offset;
	Direction m_direction;
	std::vector<Object> m_obj;
	sf::FloatRect GetRect();
};

