#include "stdafx.h"
#include "Player.h"


using namespace sf;


Player::Player(const Vector2f & position) :
	position(Vector2f(position.x + 29, position.y + 29)),
	currentFrame(0.0),
	direction(Direction::STAY)
{
	offset.x = 0.0;
	offset.y = 0.0;
	image.loadFromFile("recources/player1.png");
	texture.loadFromImage(image);

	sprite.setOrigin(18, 29);
	sprite.setTexture(texture);

	sprite.setTextureRect(IntRect(0, 0, 36, 58));

	sprite.setPosition(position);
}


void Player::GetDirection()
{

	if (Keyboard::isKeyPressed(Keyboard::Left))
	{
		direction = Direction::LEFT;
	}

	if (Keyboard::isKeyPressed(Keyboard::Right))
	{
		direction = Direction::RIGHT;
	}
	if ((Keyboard::isKeyPressed(Keyboard::Up)))
	{
		direction = Direction::UP;
	}
	if (Keyboard::isKeyPressed(Keyboard::Down))
	{
		direction = Direction::DOWN;
	}

}

void Player::Update(float time)
{
	currentFrame += 0.005 * time;
	if (direction == Direction::STAY)
	{
		GetDirection();
	}

	if (currentFrame > 2 && (direction == Direction::RIGHT || direction == Direction::LEFT))
	{
		currentFrame = 0;
	}
	if (currentFrame > 3 && (direction == Direction::UP || direction == Direction::DOWN))
	{
		currentFrame = 0;
	}

	switch (direction)
	{
	case Direction::RIGHT:
		offset.x = 0.1;
		offset.y = 0;
		sprite.setTextureRect(IntRect(36, 58 * int(currentFrame), 36, 58));
		break;
	case Direction::LEFT:
		offset.x = -0.1;
		offset.y = 0;
		sprite.setTextureRect(IntRect(108, 58 * int(currentFrame), 36, 58));
		break;
	case Direction::UP:
		offset.y = -0.1;
		offset.x = 0;
		sprite.setTextureRect(IntRect(72, 58 * int(currentFrame), 36, 58));
		break;
	case Direction::DOWN:
		offset.y = 0.1;
		offset.x = 0;
		sprite.setTextureRect(IntRect(0, 58 * int(currentFrame), 36, 58));
		break;
	case Direction::STAY:
		offset.x = 0;
		offset.y = 0;
		//sprite.setTextureRect(IntRect(0, 0, 36, 58));
		break;
	default:
		break;
	}
	if (offset.x != 0)
	{
		position.x += offset.x * time;
		if (static_cast<int>(position.x - 29) % 58 == 0)
		{
			direction = Direction::STAY;
		}
		//проверка коллизии
		CheckCollision(offset.x, 0);
	}
	if (offset.y != 0)
	{
		position.y += offset.y * time;
		if (static_cast<int>(position.y - 29) % 58 == 0)
		{
			direction = Direction::STAY;
		}
		//проверка коллизии
		CheckCollision(0, offset.y);
	}
}

void Player::InitPlayer(Level &level)
{
	obj = level.GetAllObjects();
}

sf::FloatRect Player::GetRect()
{
	return FloatRect(position.x- 29, position.y- 29, 57, 57/*58-не але иногда.57 это костыль ,я подозреваю*/);
}

void Player::CheckCollision(const float & dx, const float & dy)
{
	for (size_t i = 0; i < obj.size(); ++i)
	{
		if (GetRect().intersects(obj[i].rect) && (obj[i].name == "collision"))
		{
			direction = Direction::STAY;
			if (dy > 0)//вниз
			{
				position.y = obj[i].rect.top - 29;
			}
			if (dy < 0)//вверх
			{
				position.y = obj[i].rect.top + obj[i].rect.height + 29;
			}
			if (dx > 0)//вправо
			{
				position.x = obj[i].rect.left - 29;
			}
			if (dx < 0)//влево
			{
				position.x = obj[i].rect.left + obj[i].rect.width + 29;
			}
		}
	}
}


