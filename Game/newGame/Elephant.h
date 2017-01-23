#pragma once
class CElephant
{
public:
	CElephant(const sf::Vector2f &);

	sf::Sprite GetHero() const;
	sf::Vector2f GetPosition()const;
	sf::Vector2f GetOffset()const;
	sf::FloatRect GetRect()const;

	sf::FloatRect GetRectBonus()const;

	sf::Sprite GetBonus() const;

	void Init(Level &level);
	void SetPosition();

	void SetShow();

	bool IsStop(float playerPosition);

	bool IsRun();

	void Update(float time);

	bool IsShow();
	~CElephant();
private:
	sf::Image m_image;
	sf::Texture m_texture;
	sf::Sprite m_sprite;

	sf::Sprite m_spriteBonus;

	sf::Vector2f m_position;
	sf::Vector2f m_offset;
	Direction m_direction;

	std::vector<Object> m_obj;
	Object m_objBonus;


	float m_currentFrame;
	int m_row;

	bool m_isCollision;
	void CheckCollision(float dx, float dy);
	bool m_isShow;

};

