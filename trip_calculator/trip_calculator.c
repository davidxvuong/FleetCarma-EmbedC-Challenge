#include "trip_calculator.h"

//These two functions are simply used as the functions to be integrated via the trapezoidal rule. We are treating the integration as a black box despite having the implementation for it. This allows the numerical integration methods to be reused elsewhere if needed.
float power_function(float vehicle_power, float x) {
	//Note that power = battery voltage * battery current
	return vehicle_power;
}

float speed_function(float vehicle_speed, float x) {
	//TODO: define distance function
	return vehicle_speed;
}

void initialize_trip_event_summary() {
	trip_data.start_time_since_unix_epoch_seconds = 0;
	trip_data.duration_seconds = 0;
	trip_data.distance_travelled_meters = 0;
	trip_data.total_energy_consumed_wh = 0;
	trip_data.starting_soc = 0.0f;
	trip_data.ending_soc = 0.0f;

	is_travelling = false;
	received_first_bat_soc_signal = false;
	received_voltage = false;
	received_current = false;
	no_prior_data = false;
	prev_signal_time_seconds = 0;
}

void process_vehicle_signal(vehicle_signal_t signal) {
	uint32_t prev_time_relative;
	uint32_t current_time_relative;
	uint32_t current_unix_timestamp_seconds;

	current_unix_timestamp_seconds = signal.unix_timestamp_milliseconds / 1000;

	update_time_and_duration(current_unix_timestamp_seconds);

	prev_time_relative = prev_signal_time_seconds - trip_data.start_time_since_unix_epoch_seconds;
	current_time_relative = current_unix_timestamp_seconds - trip_data.start_time_since_unix_epoch_seconds;

	//determine appropriate type of signal, then call the corresponding update function as needed
	switch (signal.signal_type) {

		case VEHICLE_SIGNAL_TYPE_VEHICLE_SPEED:

			if (!no_prior_data)
				update_distance(prev_time_relative, current_time_relative, signal.value);
			break;

		case VEHICLE_SIGNAL_TYPE_HV_BATTERY_VOLTAGE:

			voltage = signal.value;
			received_voltage = true;
			break;

		case VEHICLE_SIGNAL_TYPE_HV_BATTERY_CURRENT:

			current = signal.value;
			received_current = true;
			break;

		case VEHICLE_SIGNAL_TYPE_HV_BATTERY_SOC:

			update_battery_soc_level(signal.value);
			break;
	}

	if (received_voltage && received_current && !no_prior_data) {
		received_current = false;
		received_voltage = false;

		update_net_energy(prev_time_relative, current_time_relative, voltage * current);
	}

}

void update_time_and_duration(uint32_t time) {
	//update duration and start time (if vehicle has not started moving yet)
	uint32_t time_diff;

	if (!is_travelling) {
		//vehicle hasn't moved before, but we got a vehicle signal, so it is now
		is_travelling = true;
		no_prior_data = true;
		prev_signal_time_seconds = time;
		trip_data.start_time_since_unix_epoch_seconds = time;
	}
	else {
		//vehicle is in motion, so determine the duration travelled thus far
		trip_data.duration_seconds = time - trip_data.start_time_since_unix_epoch_seconds;

		time_diff = time - prev_signal_time_seconds;

		if (time_diff == 2) {
			++prev_signal_time_seconds;
		}
		else if (time_diff == 1) {
			no_prior_data = false;
		}
	}
}

void update_distance(uint32_t start_time, uint32_t final_time, float speed) {
	float converted_speed;
	float result;

	//convert speed from km/h to m/s
	converted_speed = KM_H_TO_M_S_CONV_FACTOR * speed;

	result = integrate(start_time, final_time, NUM_POINTS, speed, &speed_function);

	trip_data.distance_travelled_meters += result;
}

void update_net_energy(uint32_t start_time, uint32_t final_time, float power) {
	//TODO: call integrate function

	float result;

	result = integrate(start_time, final_time, NUM_POINTS, power, &power_function);

	trip_data.total_energy_consumed_wh += result;
}

void update_battery_soc_level(float battery_level) {

	if (!received_first_bat_soc_signal) {
		received_first_bat_soc_signal = true;
		trip_data.starting_soc = battery_level;
	}

	trip_data.ending_soc = battery_level;
}

trip_event_summary_t get_trip_event_summary() {
	return trip_data;
}