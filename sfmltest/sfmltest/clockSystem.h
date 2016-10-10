#pragma once
#include "consts.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


using namespace sf;
struct ClockCharacteristics
{
	Texture clockBrand;
	Music clockTick;
};

struct ClockHands
{
	RectangleShape hourHand;
	RectangleShape minuteHand;
	RectangleShape secondsHand;
};

struct ClockSystem
{
	Font font;
	Text num[NUMBER_DIGITS];
	CircleShape dot[NUMBER_DOTS];
	Sprite clockBrandSprite;
	CircleShape clockCircle;
	CircleShape centerCircle;
	ClockHands clockHands;
	ClockCharacteristics clockApp;
};


void CreateSprite(Texture &clockBrand, Sprite &clockBrandSprite, const Vector2f &windowCenter);
void CreateDigit(int i, ClockSystem &clock, float angle, int &digit, const Vector2f &windowCenter);
void ChangeCharacteristicsDot(int i, CircleShape dot[NUMBER_DOTS], const Vector2f &windowCenter, float angle);
void CreateClockFace(const Vector2f &windowCenter, ClockSystem &clock);
void CreateOutline(CircleShape &clockCircle, const Vector2f &windowCenter);
void CreateCenterCircle(CircleShape &centerCircle, const Vector2f &windowCenter);
void CreateHands(ClockHands &clockHands, const Vector2f &windowCenter);
void CreateClock(ClockSystem &clock, const Vector2f &windowCenter, Texture &clockBrand);
void PlayClockTick(Music &clockTick);
