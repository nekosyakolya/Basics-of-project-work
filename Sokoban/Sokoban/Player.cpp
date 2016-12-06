
#include "Player.h"


using namespace sf;


Player::Player(const Vector2f & position) :
	position(position)
{

	image.loadFromFile("recources/player.png");
	texture.loadFromImage(image);

	sprite.setTexture(texture);

	sprite.setTextureRect(IntRect(0, 0, 57, 57));

	sprite.setPosition(position);
}

