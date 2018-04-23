#include <stdio.h>
// #include "trip_calculator/trip_calculator.h"
#include "trip_calculator/numerical_integration.h"

float f(float val, float x) {
	return val * x * x * x * x * x;
}

int main()
{
    // place your unit test code here

    int i;

    for (i = 1; i < 1000; i++) {
		float value = integrate(0, 2, i, 3, &f);
		printf("%f\n", value);
    }
    return 0;
}