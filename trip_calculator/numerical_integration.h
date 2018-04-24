#ifndef NUMERICAL_INTEGRATION_H
#define NUMERICAL_INTEGRATION_H

float coefficient;
float integrate(float lower_bound, float upper_bound, int num_of_points, float constant, float (*function)(float coefficient, float x));

#endif