#ifndef NUMERICAL_INTEGRATION_H
#define NUMERICAL_INTEGRATION_H

float integrate(float lower_bound, float upper_bound, int num_of_points, float value, float (*function)(float y, float x));

#endif