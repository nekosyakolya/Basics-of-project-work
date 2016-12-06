#include "Box.h"

using namespace sf;

Box::Box(const Vector2f &position, bool isState) :
	position(position),
	isStateFinal(isState)

{
	image.loadFromFile("recources/sprit.png");

	auto positionSprite = (isStateFinal) ? Vector2f(0, 114) : Vector2f(0, 57);
	texture.loadFromImage(image);

	sprite.setTexture(texture);

	sprite.setTextureRect(IntRect(static_cast<int>(positionSprite.x), static_cast<int>(positionSprite.y), 57, 57));

	sprite.setPosition(position);

}

