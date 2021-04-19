#include "CalculatorSciPixel.h"

int main()
{
	dwe::CalculatorSciPixel calcPixel;
	if (calcPixel.Construct(640, 480, 1, 1))
		calcPixel.Start();
}