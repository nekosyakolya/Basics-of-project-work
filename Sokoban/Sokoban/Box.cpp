#include "stdafx.h"
#include "Box.h"

using namespace sf;

Box::Box(const Vector2f &position, bool isState) :
	position(position),
	isStateFinal(isState)

{
	image.loadFromFile("recources/sprite.png");

	auto positionSprite = (isStateFinal) ? Vector2f(0, 116) : Vector2f(0, 58);
	texture.loadFromImage(image);

	sprite.setTexture(texture);

	sprite.setTextureRect(IntRect(static_cast<int>(positionSprite.x), static_cast<int>(positionSprite.y), 58, 58));

	sprite.setPosition(position);

}

