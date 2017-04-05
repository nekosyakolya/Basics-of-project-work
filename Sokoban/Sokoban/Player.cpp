#include "stdafx.h"
#include "Player.h"
#include "const.h"


using namespace sf;


Player::Player(const Vector2f & position) :
	position(Vector2f(position.x + (SIZE_TILE / 2), position.y + (SIZE_TILE / 2))),
	currentFrame(0.0),
	direction(Direction::STAY)
{
	offset.x = 0.0;
	offset.y = 0.0;
	image.loadFromFile("recources/player1.png");
	texture.loadFromImage(image);

	sprite.setOrigin((WIDTH_PLAYER / 2), (SIZE_TILE / 2));
	sprite.setTexture(texture);

	sprite.setTextureRect(IntRect(0, 0, WIDTH_PLAYER, SIZE_TILE));

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


void Player::CheckCentre(const float &value)
{
	if (static_cast<int>(value - (SIZE_TILE / 2)) % SIZE_TILE == 0)
	{
		direction = Direction::STAY;
	}
}

void Player::Update(float time)
{
	currentFrame += 0.005f * time;
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
		offset.x = 0.1f;
		offset.y = 0;
		sprite.setTextureRect(IntRect(WIDTH_PLAYER, SIZE_TILE * static_cast<int>(currentFrame), WIDTH_PLAYER, SIZE_TILE));
		break;
	case Direction::LEFT:
		offset.x = -0.1f;
		offset.y = 0;
		sprite.setTextureRect(IntRect(WIDTH_PLAYER * 3, SIZE_TILE * static_cast<int>(currentFrame), WIDTH_PLAYER, SIZE_TILE));
		break;
	case Direction::UP:
		offset.y = -0.1f;
		offset.x = 0;
		sprite.setTextureRect(IntRect(WIDTH_PLAYER * 2, SIZE_TILE * static_cast<int>(currentFrame), WIDTH_PLAYER, SIZE_TILE));
		break;
	case Direction::DOWN:
		offset.y = 0.1f;
		offset.x = 0;
		sprite.setTextureRect(IntRect(0, SIZE_TILE * static_cast<int>(currentFrame), WIDTH_PLAYER, SIZE_TILE));
		break;
	case Direction::STAY:
		offset.x = 0;
		offset.y = 0;
		break;
	default:
		break;
	}
	if (offset.x != 0)
	{
		position.x += offset.x * time;
		CheckCentre(position.x);
		CheckCollision(offset.x, 0);
	}
	if (offset.y != 0)
	{
		position.y += offset.y * time;
		CheckCentre(position.y);
		CheckCollision(0, offset.y);
	}
}

void Player::InitPlayer(Level &level)
{
	obj = level.GetAllObjects();
}

sf::FloatRect Player::GetRect()
{
	return FloatRect(position.x - (SIZE_TILE / 2), position.y - (SIZE_TILE / 2), 57, 57);
}

void Player::CheckCollision(const float & dx, const float & dy)
{
	for (size_t i = 0; i < obj.size(); ++i)
	{
		if (GetRect().intersects(obj[i].rect) && (obj[i].name == "collision"))
		{
			direction = Direction::STAY;
			if (dy > 0)
			{
				position.y = obj[i].rect.top - (SIZE_TILE / 2);
			}
			if (dy < 0)
			{
				position.y = obj[i].rect.top + obj[i].rect.height + (SIZE_TILE / 2);
			}
			if (dx > 0)
			{
				position.x = obj[i].rect.left - (SIZE_TILE / 2);
			}
			if (dx < 0)
			{
				position.x = obj[i].rect.left + obj[i].rect.width + (SIZE_TILE / 2);
			}
		}
	}
}


