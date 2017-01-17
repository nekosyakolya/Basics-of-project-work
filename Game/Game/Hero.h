#pragma once


#include <SFML/Graphics.hpp>

enum struct Direction
{
	LEFT = -1,
	UP = 0,
	RIGHT = 1,
	JUMP = 2
};


class CHero
{
public:
	CHero(const sf::Vector2f &);
	sf::Sprite GetHero() const;
	sf::Vector2f GetPosition()const;
	sf::Vector2f GetOffset()const;
	sf::FloatRect GetRect()const;


	virtual void Update(float time) = 0;

	void SetPosition();


	virtual void UpdatePosition(float dx) = 0;

	bool IsFinalState()const;

	void SetFinalState();

	void Init(Level &level);

	virtual ~CHero();
protected:
	std::vector<Object> m_obj;

	sf::Vector2f m_position;
	sf::Vector2f m_offset;
	Direction m_direction;
	bool m_isFinalState = false;
	float m_delta;
	float m_currentFrame;


	void CheckCollision(float dx, float dy);

	sf::Image m_image;
	sf::Texture m_texture;
	sf::Sprite m_sprite;
};

