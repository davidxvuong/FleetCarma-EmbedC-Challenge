#ifndef TRIP_CALCULATOR_H
#define TRIP_CALCULATOR_H

#include <stdint.h>
#include "vehicle_signal_t.h"
#include "numerical_integration.h"

typedef struct {
    uint32_t start_time_since_unix_epoch_seconds;
    uint32_t duration_seconds;
    uint32_t distance_travelled_meters;
    uint32_t total_energy_consumed_wh;
    float starting_soc;
    float ending_soc;
}trip_event_summary_t;

trip_event_summary_t trip_data;

//internal functions used by the integration
float energy_function(float vehicle_power, float x);
float distance_function(float vehicle_speed, float x);

void process_vehicle_signal(vehicle_signal_t signal);
void update_start_time(float time);
void update_duration(float time);
void update_distance(float distance);
void update_start_battery_level(float battery_level);
void update_end_battery_level(float battery_level);

trip_event_summary_t get_trip_event_summary();

#endif