#define _CRT_SECURE_NO_WARNINGS

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
int main(int, char *[])
{
	const float g = 9.8f;
	float timeReachMaxHeight;
	int heightJump;
	printf("Height jump: ");
	if (scanf_s("%d", &heightJump) == 0)
	{
		printf("\n" "expected floating-point number" "\n");
		exit(1);
	}
	// v(t) == v0 - g * t
	// v0 = g * T
	// s(t) == v0 * t - 0.5 * g * t * t
	timeReachMaxHeight = sqrt(heightJump * 2 / g);
	printf("Time point when height is at maximum =%f\n", timeReachMaxHeight);
	bool flag = false;
	for (float currTime = 0; currTime < timeReachMaxHeight * 2; currTime += 0.1f)
	{
		if (currTime > timeReachMaxHeight && !flag)
		{
			flag = true;
			float V0 = g * timeReachMaxHeight;
			float currHeight = V0 * timeReachMaxHeight - 0.5 * g * timeReachMaxHeight * timeReachMaxHeight;
			printf("t=%f, s=%f\n", timeReachMaxHeight, currHeight);
		}
		float V0 = g * timeReachMaxHeight;
		float currHeight = V0 * currTime - 0.5 * g * currTime * currTime;
		printf("t=%f, s=%f\n", currTime, currHeight);
	}

	float V0 = g * timeReachMaxHeight;
	float currHeight = V0 * (timeReachMaxHeight * 2) - 0.5 * g * (timeReachMaxHeight * 2) * (timeReachMaxHeight * 2);
	printf("t=%f, s=%f\n", timeReachMaxHeight * 2, currHeight);

	// TODO: remove system("pause") and never use it again.
	//system("pause");
	printf("Press any key... ");
	_getch();

	return 0;
}

