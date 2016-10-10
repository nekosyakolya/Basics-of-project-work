#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <string>
#include <sstream>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace sf;

template <typename T>
std::string toString(T val)
{
	std::ostringstream oss;
	oss << val;
	return oss.str();
}


// Define some variables and constants
static const unsigned int SCREEN_WIDTH = 1200;
static const unsigned int SCREEN_HEIGHT = 700;
static const float CLOCK_CIRCLE_SIZE = 250;
static const float CLOCK_CIRCLE_THICKNESS = 2;
static const unsigned int NUMBER_DOTS = 60;
static const unsigned int NUMBER_DIGITS = 12;

struct ClockCharacteristics
{
	Texture clockBrand;
	Music clockTick;
};

struct ClockSystem
{

	Font font;
	Text num[NUMBER_DIGITS];
	CircleShape dot[NUMBER_DOTS];
	Sprite clockBrandSprite;

	CircleShape clockCircle;
	CircleShape centerCircle;

	RectangleShape hourHand;
	RectangleShape minuteHand;
	RectangleShape secondsHand;

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

int GetDigit(int digit)
{
	if (digit == NUMBER_DIGITS)
	{
		digit = 0;
	}
	return ++digit;
}

void CreateDigit(int i, Text num[NUMBER_DIGITS], float angle, Vector2f &digitPosition,
	Font &font, int &digit, const Vector2f &windowCenter)
{
	int indexNum = i / 5;
	num[indexNum].setFont(font);
	num[indexNum].setFillColor(Color::Black);
	num[indexNum].setStyle(Text::Bold);
	num[indexNum].setString(toString(digit));
	num[indexNum].setCharacterSize(38);
	digitPosition.x = (CLOCK_CIRCLE_SIZE - 50) * cos(angle);
	digitPosition.y = (CLOCK_CIRCLE_SIZE - 50) * sin(angle);

	Vector2f positionText(digitPosition + windowCenter);

	num[indexNum].setPosition(positionText.x, positionText.y - num[indexNum].getGlobalBounds().height / 2.0f);

	num[indexNum].setOrigin(num[indexNum].getGlobalBounds().width / 2.0f,
		num[indexNum].getGlobalBounds().height / 2.0f);

	digit = GetDigit(digit);
}


void CreateClockFace(const Vector2f &windowCenter, Text num[NUMBER_DIGITS], 
	CircleShape dot[NUMBER_DOTS], Font &font)
{
	float angle = 0.0;

	int digit = 3;
	
	Vector2f dotPosition(0, 0);
	Vector2f digitPosition(0, 0);

	for (int i = 0; i < NUMBER_DOTS; i++)
	{
		dotPosition.x = (CLOCK_CIRCLE_SIZE - 10) * cos(angle);
		dotPosition.y = (CLOCK_CIRCLE_SIZE - 10) * sin(angle);
		if (i % 5 == 0)
		{
			dot[i] = CircleShape(7);
			CreateDigit(i, num, angle, digitPosition, font, digit, windowCenter);
		}
		else
		{
			dot[i] = CircleShape(3);
		}

		dot[i].setFillColor(Color::Black);
		dot[i].setOrigin(dot[i].getGlobalBounds().width / 2.0f, dot[i].getGlobalBounds().height / 2.0f);
		dot[i].setPosition(dotPosition.x + windowCenter.x, dotPosition.y + windowCenter.y);
		angle += float((M_PI * 2) / 60);
	}

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


void CreateOutline(CircleShape &clockCircle, const Vector2f &windowCenter)
{
	clockCircle.setRadius(CLOCK_CIRCLE_SIZE);
	clockCircle.setPointCount(100);
	clockCircle.setOutlineThickness(CLOCK_CIRCLE_THICKNESS);
	clockCircle.setOutlineColor(Color::Black);
	clockCircle.setOrigin(clockCircle.getGlobalBounds().width / 2.0f, clockCircle.getGlobalBounds().height / 2.0f);
	clockCircle.setPosition(windowCenter.x  + CLOCK_CIRCLE_THICKNESS,
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



void CreateHands(RectangleShape &hourHand, RectangleShape &minuteHand, RectangleShape &secondsHand, 
	const Vector2f &windowCenter)
{

	hourHand.setSize(Vector2f(7, 180));
	minuteHand.setSize(Vector2f(5, 230));
	secondsHand.setSize(Vector2f(2, 270));

	hourHand.setFillColor(Color::Black);
	minuteHand.setFillColor(Color(128, 128, 128));
	secondsHand.setFillColor(Color(62, 133, 4));

	hourHand.setOrigin(hourHand.getGlobalBounds().width / 2.0f, hourHand.getGlobalBounds().height);
	minuteHand.setOrigin(minuteHand.getGlobalBounds().width / 2.0f, minuteHand.getGlobalBounds().height);
	secondsHand.setOrigin(secondsHand.getGlobalBounds().width / 2.0f,
		secondsHand.getGlobalBounds().height - 25);

	hourHand.setPosition(windowCenter);
	minuteHand.setPosition(windowCenter);
	secondsHand.setPosition(windowCenter);
}


void TurnHands(RectangleShape &hourHand, RectangleShape &minuteHand, RectangleShape &secondsHand)
{
	std::time_t currentTime = std::time(NULL);

	struct tm * ptm = localtime(&currentTime);

	hourHand.setRotation(float(ptm->tm_hour * 30) + (float(ptm->tm_min / 2)));
	minuteHand.setRotation(float(ptm->tm_min * 6) + (float(ptm->tm_sec / 12)));
	secondsHand.setRotation(float(ptm->tm_sec * 6));
}


void DisplayClock(RenderWindow &window, ClockSystem &clock)
{
	window.draw(clock.clockCircle);
	DisplayClockFace(window, clock.num, clock.dot);

	window.draw(clock.clockBrandSprite);

	window.draw(clock.hourHand);
	window.draw(clock.minuteHand);
	window.draw(clock.secondsHand);

	window.draw(clock.centerCircle);
}


void CreateClock(ClockSystem &clock, const Vector2f &windowCenter, Texture &clockBrand)
{
	CreateSprite(clockBrand, clock.clockBrandSprite, windowCenter);

	CreateClockFace(windowCenter, clock.num, clock.dot, clock.font);

	CreateOutline(clock.clockCircle, windowCenter);

	CreateCenterCircle(clock.centerCircle, windowCenter);

	CreateHands(clock.hourHand, clock.minuteHand, clock.secondsHand, windowCenter);
}


void HandleEvent(RenderWindow &window)
{
	Event event;
	while (window.pollEvent(event))
	{
		// Window closed: exit
		if (event.type == Event::Closed)
		{
			window.close();
		}
	}
}

void ProcessClock(RenderWindow &window, ClockSystem &clock)
{
	while (window.isOpen())
	{
		HandleEvent(window);
		
		TurnHands(clock.hourHand, clock.minuteHand, clock.secondsHand);

		window.clear(Color::White);

		DisplayClock(window, clock);

		window.display();
	}
}


void PlayClockTick(Music &clockTick)
{
	clockTick.setLoop(true);
	clockTick.play();
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
	ClockCharacteristics clockApp;

	ContextSettings settings;
	settings.antialiasingLevel = 8;

	RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML Analog Clock", Style::Close, settings);

	Vector2f windowCenter = Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
	
	CheckOpenResources(clock);
	
	PlayClockTick(clock.clockApp.clockTick);

	CreateClock(clock, windowCenter, clock.clockApp.clockBrand);

	ProcessClock(window, clock);

	return EXIT_SUCCESS;
}