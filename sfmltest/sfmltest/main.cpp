#include "stdafx.h"
#include "clockSystem.h"
#include <ctime>
#include "consts.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;


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