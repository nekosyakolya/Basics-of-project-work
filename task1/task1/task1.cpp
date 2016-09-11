
#include "stdafx.h"
#include <conio.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

// This program takes max jump height from input and prints
// jump height for every time point with step 0.1 seconds.
// Program should print all time points when height is min and max.
//
// TODO: Fix all warnings on high warning level (/W4, -Wall -Wextra).
// TODO: Rename variables and split to several functions,
// see also https://ps-group.github.io/sfml/coding_conventions.html
// TODO: fix negative height values, fix heigh values higher than max height.

void PrintCurrentState(const float G, const float time, const float v0)
{
	double currHeight = v0 * time - 0.5 * G * time * time;
	if (currHeight >= 0)
	{
		printf("t=%f, s=%f\n", time, currHeight);
	}
	return;
}

int main(int, char *[])
{
	const float G = 9.8f;
	float timeReachMaxHeight;
	int heightJump;
	do
	{
		printf("Height jump: ");
		if (scanf_s("%d", &heightJump) == 0)
		{
			printf("\n" "expected floating-point number" "\n");
			exit(1);
		}
	    if (heightJump <= 0);
		{
			printf("error" "\n");
		}
	}
	while (heightJump <= 0);
	
	
		timeReachMaxHeight = sqrt(heightJump * 2 / G);
		printf("Time point when height is at maximum = %f\n", timeReachMaxHeight);
		bool flag = false;
		for (float currTime = 0; currTime < timeReachMaxHeight * 2; currTime += 0.1f)
		{
			float v0 = G * timeReachMaxHeight;
			if (currTime > timeReachMaxHeight && !flag)
			{
				flag = true;
				PrintCurrentState(G, timeReachMaxHeight, v0);
			}
			PrintCurrentState(G, currTime, v0);
		}

		float v0 = G * timeReachMaxHeight;
		PrintCurrentState(G, timeReachMaxHeight * 2, v0);

	
	printf("Press any key... ");
	_getch();
	return 0;
}

