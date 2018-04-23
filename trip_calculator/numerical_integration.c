#include "numerical_integration.h"

//Using trapezoidal rule: http://web2.uwindsor.ca/courses/engineering/ahmed/PDF%20FILES%20FOR%20C++/Chapter%20X%20C++.pdf
float integrate(float lower_bound, float upper_bound, int num_of_points, float value, float (*function)(float y, float x)) {
	float area;
	float dx;
	float x;
	float sum;
	int i;

	dx = (upper_bound - lower_bound) / num_of_points;
	x = lower_bound + dx;

	for (i = 1; i < num_of_points; i++) {
		sum += function(value, x);
		x += dx;
	}

	area = (2.0 * sum) + function(value, upper_bound) + function(value, lower_bound);

	area *= dx/2.0;

	return area;

}