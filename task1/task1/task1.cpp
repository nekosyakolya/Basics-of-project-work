#include "stdafx.h"
#include <conio.h>
#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>
#include <cfloat>


// This program takes max jump height from input and prints
// jump height for every time point with step 0.1 seconds.
// Program should print all time points when height is min and max.

using namespace std;

static const float G = 9.8f;

void PrintCurrentState(const float &, const float &);
void DisplayStates(const float &);
int GetHeightJump();
bool IsValidHeight(int);

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
	bool printedMaxState = false;
	float v0 = G * timeReachMaxHeight;
	for (float currTime = 0; currTime < timeReachMaxHeight * 2; currTime += 0.1f)
	{
		if (currTime > timeReachMaxHeight && !printedMaxState)
		{
			printedMaxState = true;
			PrintCurrentState(timeReachMaxHeight, v0);
		}
		PrintCurrentState(currTime, v0);
	}
	PrintCurrentState(timeReachMaxHeight * 2, v0);
}

bool IsValidHeight(int heightJump)
{ 
	return ((heightJump >= 0) && (heightJump < INT_MAX));
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
			cout << "entered is not a number\n";
			exit(EXIT_FAILURE);
		}
		if (!IsValidHeight(heightJump))
		{
			cout << "error!enter a positive number\n";
		}
	} 
	while (!IsValidHeight(heightJump));
	return heightJump;
}

int main()
{
	int heightJump = GetHeightJump();
	float timeReachMaxHeight = sqrt(heightJump * 2 / G);

	DisplayStates(timeReachMaxHeight);
	return EXIT_SUCCESS;
}

