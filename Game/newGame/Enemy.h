#pragma once

#include "Hero.h"

class CEnemy :
	public CHero
{
public:
	CEnemy(const sf::Vector2f &position, float delta);
	void Update(float time) override;
	void UpdatePosition(float dx) override;

	bool Check(float dx);


	sf::Sprite GetRectProtection()const;

	void SetProtection();

	bool IsJump() const;
	~CEnemy();
private:
	sf::Image m_imageProtection;
	sf::Texture m_textureProtection;
	sf::Sprite m_spriteProtection;

	bool m_isJump;
};

