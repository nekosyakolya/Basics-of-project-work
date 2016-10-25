#pragma once
#include "consts.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

struct ClockCharacteristics
{
	sf::Texture clockBrand;
	sf::Music clockTick;
};

struct ClockHands
{
	sf::RectangleShape hourHand;
	sf::RectangleShape minuteHand;
	sf::RectangleShape secondsHand;
};

struct ClockSystem
{
	sf::Font font;
	sf::Text num[NUMBER_DIGITS];
	sf::CircleShape dot[NUMBER_DOTS];
	sf::Sprite clockBrandSprite;
	sf::CircleShape clockCircle;
	sf::CircleShape centerCircle;
	ClockHands clockHands;
	ClockCharacteristics clockApp;
};

void CreateSprite(sf::Texture &clockBrand, sf::Sprite &clockBrandSprite, const sf::Vector2f &windowCenter);
void CreateDigit(int i, ClockSystem &clock, float angle, const sf::Vector2f &windowCenter);
void ChangeCharacteristicsDot(int i, sf::CircleShape dot[NUMBER_DOTS], const sf::Vector2f &windowCenter, float angle);
void CreateClockFace(const sf::Vector2f &windowCenter, ClockSystem &clock);
void CreateOutline(sf::CircleShape &clockCircle, const sf::Vector2f &windowCenter);
void CreateCenterCircle(sf::CircleShape &centerCircle, const sf::Vector2f &windowCenter);
void CreateHands(ClockHands &clockHands, const sf::Vector2f &windowCenter);
void CreateClock(ClockSystem &clock, const sf::Vector2f &windowCenter, sf::Texture &clockBrand);
void PlayClockTick(sf::Music &clockTick);