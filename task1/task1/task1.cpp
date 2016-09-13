
#include "stdafx.h"
#include <conio.h>
#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>
#include <cfloat>
#define G 9.8f

// This program takes max jump height from input and prints
// jump height for every time point with step 0.1 seconds.
// Program should print all time points when height is min and max.

using namespace std;
void PrintCurrentState(const float &time, const float &v0);
void DisplayStates(const float &timeReachMaxHeigh);
int GetHeightJump();
void Pause();


void PrintCurrentState(const float &time, const float &v0)
{
	double currHeight = v0 * time - 0.5 * G * time * time;
	if (currHeight >= 0)
	{
		cout << "t=" << fixed << setprecision(6) << time << ", h=" << fixed << setprecision(6) << currHeight << "\n";
	}
}

void DisplayStates(const float &timeReachMaxHeight)
{
	cout << "Time point when height is at maximum = " << fixed << setprecision(6) << timeReachMaxHeight << "\n";
	bool printMaxState = false;
	float v0 = G * timeReachMaxHeight;
	for (float currTime = 0; currTime < timeReachMaxHeight * 2; currTime += 0.1f)
	{
		if (currTime > timeReachMaxHeight && !printMaxState)
		{
			printMaxState = true;
			PrintCurrentState(timeReachMaxHeight, v0);
		}
		PrintCurrentState(currTime, v0);
	}
	PrintCurrentState(timeReachMaxHeight * 2, v0);
}

int GetHeightJump()
{
	int heightJump;
	do
	{
		cout << "Height jump: ";
		cin >> heightJump;
		cin.clear();
		if (cin.get() != '\n')
		{
			cout << "expected floating-point number \n";
			exit(1);
		}
		if ((heightJump <= 0) || (heightJump > INT_MAX))
		{
			cout << "error!\n";
		}
	} 
	while ((heightJump <= 0) || (heightJump > INT_MAX));
	return heightJump;
}

void Pause()
{
	cout << "Press any key...\n";
	_getch();
}

int main()
{
	int heightJump = GetHeightJump();
	float timeReachMaxHeight = sqrt(heightJump * 2 / G);

	DisplayStates(timeReachMaxHeight);
	Pause();
	return 0;
}

