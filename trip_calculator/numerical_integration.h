#ifndef NUMERICAL_INTEGRATION_H
#define NUMERICAL_INTEGRATION_H

#include <stdint.h>
#include <stdbool.h>

float coefficient;
float integrate(uint32_t lower_bound, uint32_t upper_bound, int num_of_points, float constant, float (*function)(float coefficient, float x));

#endif