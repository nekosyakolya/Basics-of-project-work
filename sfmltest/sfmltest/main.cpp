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

struct ClockSystem
{

	Font font;
	Text num[NUMBER_DIGITS];
	CircleShape dot[NUMBER_DOTS];
	Sprite clockBrandSprite;

	RectangleShape hourHand;
	RectangleShape minuteHand;
	RectangleShape secondsHand;
};



void CreateSprite(Texture &clockBrand, Sprite &clockBrandSprite, RenderWindow &window)
{
	clockBrand.setSmooth(true);
	clockBrandSprite.setTexture(clockBrand);
	clockBrandSprite.setOrigin(clockBrandSprite.getTextureRect().left + clockBrandSprite.getTextureRect().width / 2.0f,
		clockBrandSprite.getTextureRect().top + clockBrandSprite.getTextureRect().height / 2.0f);

	clockBrandSprite.setPosition(float(window.getSize().x / 2), float(window.getSize().y - 280));

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


void CreateClockFace(const Vector2f &windowCenter, RenderWindow &window, Text num[NUMBER_DIGITS], 
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
		dot[i].setPosition(dotPosition.x + window.getSize().x / 2.0f, dotPosition.y + window.getSize().y / 2.0f);
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


void CreateOutline(CircleShape &clockCircle, RenderWindow &window)
{
	clockCircle.setRadius(CLOCK_CIRCLE_SIZE);
	clockCircle.setPointCount(100);
	clockCircle.setOutlineThickness(CLOCK_CIRCLE_THICKNESS);
	clockCircle.setOutlineColor(Color::Black);
	clockCircle.setOrigin(clockCircle.getGlobalBounds().width / 2.0f, clockCircle.getGlobalBounds().height / 2.0f);
	clockCircle.setPosition(window.getSize().x / 2.0f + CLOCK_CIRCLE_THICKNESS,
		window.getSize().y / 2.0f + CLOCK_CIRCLE_THICKNESS);
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
	DisplayClockFace(window, clock.num, clock.dot);

	window.draw(clock.clockBrandSprite);

	window.draw(clock.hourHand);
	window.draw(clock.minuteHand);
	window.draw(clock.secondsHand);
}

int main()
{
	ClockSystem clock;
	// Set multisampling level
	ContextSettings settings;
	settings.antialiasingLevel = 8;

	// Create the window of the application
	RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML Analog Clock", Style::Close, settings);

	// Define windowCenter which gets the center of the window here, right after creating window
	Vector2f windowCenter = Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
	
	Texture clockBrand;
	if (!clockBrand.loadFromFile("resources/clock-brand.png"))
	{
		return EXIT_FAILURE;
	}

	CreateSprite(clockBrand, clock.clockBrandSprite, window);

	if (!clock.font.loadFromFile("resources/CyrilicOld.TTF"))
	{
		return EXIT_FAILURE;
	}
	CreateClockFace(windowCenter, window, clock.num, clock.dot, clock.font);

	// Create outline of the clock
	CircleShape clockCircle;
	CreateOutline(clockCircle, window);
	
	CircleShape centerCircle;
	CreateCenterCircle(centerCircle, windowCenter);

	CreateHands(clock.hourHand, clock.minuteHand, clock.secondsHand, windowCenter);

	
	// Create sound effect
	Music clockTick;
	if (!clockTick.openFromFile("resources/clock-1.wav"))
	{
		return EXIT_FAILURE;
	}
	clockTick.setLoop(true);
	clockTick.play();
		
	clockCircle.setTextureRect(IntRect(40, 0, 900, 900));

	while (window.isOpen())
	{
		// Handle events
		Event event;
		while (window.pollEvent(event))
		{
			// Window closed: exit
			if (event.type == Event::Closed)
			{
				window.close();
			}
		}

		TurnHands(clock.hourHand, clock.minuteHand, clock.secondsHand);
	
		// Clear the window
		window.clear(Color::White);

		window.draw(clockCircle);
		

		DisplayClock(window, clock);
		
		window.draw(centerCircle);

		window.display();
	}

	return EXIT_SUCCESS;
}