#include "numerical_integration.h"

//Using trapezoidal rule: http://web2.uwindsor.ca/courses/engineering/ahmed/PDF%20FILES%20FOR%20C++/Chapter%20X%20C++.pdf
//For the purposes of this program, the we only need one coefficient. If we needed more, the coefficient will be modified to be a array of coefficients
float integrate(uint32_t lower_bound, uint32_t upper_bound, int num_of_points, float constant, float (*function)(float coefficient, float x)) {
	float area;
	float dx;
	float x;
	float sum;
	int i;

	dx = (upper_bound - lower_bound) / num_of_points;
	x = lower_bound + dx;

	for (i = 1; i < num_of_points; i++) {
		sum += function(constant, x);
		x += dx;
	}

	area = (2.0 * sum) + function(constant, upper_bound) + function(constant, lower_bound);

	area *= dx/2.0;

	return area;

}