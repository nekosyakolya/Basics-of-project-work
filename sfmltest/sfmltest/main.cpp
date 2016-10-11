#include "stdafx.h"
#include <ctime>
#include "consts.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <sstream>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace sf;
using namespace std;

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

void CreateSprite(Texture &clockBrand, Sprite &clockBrandSprite, const Vector2f &windowCenter)
{
	clockBrand.setSmooth(true);
	clockBrandSprite.setTexture(clockBrand);
	clockBrandSprite.setOrigin(clockBrandSprite.getTextureRect().left + clockBrandSprite.getTextureRect().width / 2.0f,
		clockBrandSprite.getTextureRect().top + clockBrandSprite.getTextureRect().height / 2.0f);

	clockBrandSprite.setPosition(float(windowCenter.x), float((windowCenter.y * 2.f) - 280));

	clockBrandSprite.scale(Vector2f(0.3f, 0.3f));
}

Vector2f GetPosition(float angle, int offset)
{
	return Vector2f((CLOCK_CIRCLE_SIZE - offset) * cos(angle), (CLOCK_CIRCLE_SIZE - offset) * sin(angle));
}

void CreateDigit(int i, ClockSystem &clock, float angle, const Vector2f &windowCenter)
{
	int digit = i / 5;
	clock.num[digit].setFont(clock.font);
	clock.num[digit].setFillColor(Color::Black);
	clock.num[digit].setStyle(Text::Bold);
	clock.num[digit].setString(to_string(digit + 1));
	clock.num[digit].setCharacterSize(38);
	Vector2f digitPosition = GetPosition(angle, 50);
	Vector2f positionText(digitPosition + windowCenter);
	clock.num[digit].setPosition(positionText.x, positionText.y - clock.num[digit].getGlobalBounds().height / 2.0f);

	clock.num[digit].setOrigin(clock.num[digit].getGlobalBounds().width / 2.0f,
		clock.num[digit].getGlobalBounds().height / 2.0f);
}


void ChangeCharacteristicsDot(int i, CircleShape dot[NUMBER_DOTS], const Vector2f &windowCenter, float angle)
{
	Vector2f dotPosition = GetPosition(angle, 10);
	dot[i].setFillColor(Color::Black);
	dot[i].setOrigin(dot[i].getGlobalBounds().width / 2.0f, dot[i].getGlobalBounds().height / 2.0f);
	dot[i].setPosition(dotPosition.x + windowCenter.x, dotPosition.y + windowCenter.y);
}



void CreateClockFace(const Vector2f &windowCenter, ClockSystem &clock)
{
	float angle = -1.0472f;
	const float RADIUS_BIG_DOT = 7;

	for (int i = 0; i < NUMBER_DOTS; ++i)
	{
		if (i % 5 == 0)
		{
			clock.dot[i] = CircleShape(RADIUS_BIG_DOT);
			CreateDigit(i, clock, angle, windowCenter);
		}
		else
		{
			clock.dot[i] = CircleShape(RADIUS_BIG_DOT - 4);
		}
		ChangeCharacteristicsDot(i, clock.dot, windowCenter, angle);
		angle += float((M_PI * 2) / NUMBER_DOTS);
	}
}


void CreateOutline(CircleShape &clockCircle, const Vector2f &windowCenter)
{
	clockCircle.setRadius(CLOCK_CIRCLE_SIZE);
	clockCircle.setPointCount(100);
	clockCircle.setOutlineThickness(CLOCK_CIRCLE_THICKNESS);
	clockCircle.setOutlineColor(Color::Black);
	clockCircle.setOrigin(clockCircle.getGlobalBounds().width / 2.0f, clockCircle.getGlobalBounds().height / 2.0f);
	clockCircle.setPosition(windowCenter.x + CLOCK_CIRCLE_THICKNESS,
		windowCenter.y + CLOCK_CIRCLE_THICKNESS);

	clockCircle.setTextureRect(IntRect(40, 0, 900, 900));
}

void CreateCenterCircle(CircleShape &centerCircle, const Vector2f &windowCenter)
{
	centerCircle.setRadius(10);
	centerCircle.setPointCount(100);
	centerCircle.setFillColor(Color(62, 133, 4));
	centerCircle.setOrigin(centerCircle.getGlobalBounds().width / 2.0f, centerCircle.getGlobalBounds().height / 2.0f);
	centerCircle.setPosition(windowCenter);
}



void CreateHands(ClockHands &clockHands, const Vector2f &windowCenter)
{
	clockHands.hourHand.setSize(Vector2f(7, 180));
	clockHands.minuteHand.setSize(Vector2f(5, 230));
	clockHands.secondsHand.setSize(Vector2f(2, 270));

	clockHands.hourHand.setFillColor(Color::Black);
	clockHands.minuteHand.setFillColor(Color(128, 128, 128));
	clockHands.secondsHand.setFillColor(Color(62, 133, 4));

	clockHands.hourHand.setOrigin(clockHands.hourHand.getGlobalBounds().width / 2.0f, clockHands.hourHand.getGlobalBounds().height);
	clockHands.minuteHand.setOrigin(clockHands.minuteHand.getGlobalBounds().width / 2.0f, clockHands.minuteHand.getGlobalBounds().height);
	clockHands.secondsHand.setOrigin(clockHands.secondsHand.getGlobalBounds().width / 2.0f,
		clockHands.secondsHand.getGlobalBounds().height - 25);

	clockHands.hourHand.setPosition(windowCenter);
	clockHands.minuteHand.setPosition(windowCenter);
	clockHands.secondsHand.setPosition(windowCenter);
}

void CreateClock(ClockSystem &clock, const Vector2f &windowCenter, Texture &clockBrand)
{
	CreateSprite(clockBrand, clock.clockBrandSprite, windowCenter);

	CreateClockFace(windowCenter, clock);

	CreateOutline(clock.clockCircle, windowCenter);

	CreateCenterCircle(clock.centerCircle, windowCenter);

	CreateHands(clock.clockHands, windowCenter);
}


void PlayClockTick(Music &clockTick)
{
	clockTick.setLoop(true);
	clockTick.play();
}


void DisplayClockFace(RenderWindow &window, Text num[NUMBER_DIGITS], CircleShape dot[NUMBER_DOTS])
{
	for (int i = 0; i < NUMBER_DOTS; ++i)
	{
		if (i % 5 == 0)
		{
			window.draw(num[i/5]);
		}
		window.draw(dot[i]);
	}
}

void TurnHands(ClockHands &clockHands)
{
	time_t currentTime = time(NULL);
	struct tm * ptm = localtime(&currentTime);

	clockHands.hourHand.setRotation(float(ptm->tm_hour * 30) + (float(ptm->tm_min / 2)));
	clockHands.minuteHand.setRotation(float(ptm->tm_min * 6) + (float(ptm->tm_sec / 12)));
	clockHands.secondsHand.setRotation(float(ptm->tm_sec * 6));
}

void DisplayClock(RenderWindow &window, ClockSystem &clock)
{
	window.clear(Color::White);

	window.draw(clock.clockCircle);
	DisplayClockFace(window, clock.num, clock.dot);

	window.draw(clock.clockBrandSprite);
	window.draw(clock.clockHands.hourHand);
	window.draw(clock.clockHands.minuteHand);
	window.draw(clock.clockHands.secondsHand);
	window.draw(clock.centerCircle);
	window.display();
}

void HandleEvents(RenderWindow &window)
{
	Event event;
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed)
		{
			window.close();
		}
	}
}

void EnterClocksGameLoop(RenderWindow &window, ClockSystem &clock)
{
	while (window.isOpen())
	{
		HandleEvents(window);
		
		TurnHands(clock.clockHands);

		DisplayClock(window, clock);

	}
}

void CheckOpenResources(ClockSystem &clock)
{
	bool error = false;
	if (!clock.clockApp.clockBrand.loadFromFile("resources/clock-brand.png"))
	{
		error = true;
	}

	if (!clock.font.loadFromFile("resources/CyrilicOld.TTF"))
	{
		error = true;
	}

	if (!clock.clockApp.clockTick.openFromFile("resources/clock-1.wav"))
	{
		error = true;
	}
	if (error)
	{
		exit(EXIT_FAILURE);
	}
}


int main()
{
	ClockSystem clock;

	ContextSettings settings;
	settings.antialiasingLevel = LEVEL_ANTIALIASING;

	RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML Analog Clock", Style::Close, settings);

	Vector2f windowCenter = Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
	
	CheckOpenResources(clock);
	
	PlayClockTick(clock.clockApp.clockTick);

	CreateClock(clock, windowCenter, clock.clockApp.clockBrand);

	EnterClocksGameLoop(window, clock);

	return EXIT_SUCCESS;
}