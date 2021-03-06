#ifndef TRIP_CALCULATOR_H
#define TRIP_CALCULATOR_H

#include <stdint.h>
#include <stdbool.h>
#include "vehicle_signal_t.h"
#include "numerical_integration.h"

//Determines how to slice up the area under the curve
#define NUM_POINTS 1
#define KM_H_TO_M_S_CONV_FACTOR (50.0/3.0)

typedef struct {
    uint32_t start_time_since_unix_epoch_seconds;
    uint32_t duration_seconds;
    uint32_t distance_travelled_meters;
    uint32_t total_energy_consumed_wh;
    float starting_soc;
    float ending_soc;
}trip_event_summary_t;

trip_event_summary_t trip_data;

//control/intermediate data variables for trip event summary
bool is_travelling;
bool no_prior_data;
bool received_first_bat_soc_signal;
bool received_voltage;
bool received_current;
uint32_t prev_signal_time_seconds;
float voltage;
float current;

//internal functions used by the integration
float power_function(float vehicle_power, float x);
float speed_function(float vehicle_speed, float x);

void initialize_trip_event_summary();
void process_vehicle_signal(vehicle_signal_t signal);
void update_time_and_duration(uint32_t time);
void update_distance(uint32_t start_time, uint32_t final_time, float speed);
void update_net_energy(uint32_t start_time, uint32_t final_time, float power);
void update_battery_soc_level(float battery_level);

trip_event_summary_t get_trip_event_summary();

#endif