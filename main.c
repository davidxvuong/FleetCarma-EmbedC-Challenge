#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>
#include "trip_calculator/trip_calculator.h"

#define BUF_SIZE 1000

typedef enum {
	TIMESTAMP,
	HV_BAT_VOLTAGE,
	HV_BAT_CURRENT,
	HV_BAT_SOC,
	VEHICLE_SPEED
} counter_t;

int main()
{	
	//Set up the test environment
	FILE* ptr_file;
	char buf[BUF_SIZE];
	char* token;
	bool ignore_first_line = true;
	vehicle_signal_t signal;
	int count;
	uint64_t unix_time_ms;
	trip_event_summary_t trip_summary;

	ptr_file = fopen("demo_trip_data.csv", "r");

	if (!ptr_file) {
		printf("Unable to open file\n");
		return 1;
	}

	//initialize trip summary
	initialize_trip_event_summary();

	while(fgets(buf, BUF_SIZE, ptr_file) != NULL) {
		count = 0;

		//first line is just the column headers...can safely ignore it
		if (ignore_first_line) {

			ignore_first_line = false;
			continue;
		}

		//split each line, and figure out what kind of value it is
		token = strtok(buf, ",");

		while (token != NULL) {

			switch (count) {
				case TIMESTAMP:
					//1st value of every line is the timestamp. This will be needed later. Store this somewhere for now

					sscanf(token, "%" SCNu64, &unix_time_ms);
					break;
				case HV_BAT_VOLTAGE:

					signal.signal_type = VEHICLE_SIGNAL_TYPE_HV_BATTERY_VOLTAGE;
					break;
				case HV_BAT_CURRENT:

					signal.signal_type = VEHICLE_SIGNAL_TYPE_HV_BATTERY_CURRENT;
					break;
				case HV_BAT_SOC:

					signal.signal_type = VEHICLE_SIGNAL_TYPE_HV_BATTERY_SOC;
					break;
				case VEHICLE_SPEED:

					signal.signal_type = VEHICLE_SIGNAL_TYPE_VEHICLE_SPEED;
					break;
			}

			if (count != TIMESTAMP) {
				signal.unix_timestamp_milliseconds = unix_time_ms;
				signal.value = atof(token);

				process_vehicle_signal(signal);
			}

			++count;
			token = strtok(NULL, ",");
		}
	}

	fclose(ptr_file);

   	printf("Done. Trip event summary shown below:\n");

	trip_summary = get_trip_event_summary();

	printf("Start time: %u\n", trip_summary.start_time_since_unix_epoch_seconds);
	printf("Duration: %u\n", trip_summary.duration_seconds);
	printf("Total distance travelled: %u\n", trip_summary.distance_travelled_meters);
	printf("Total energy consumed: %u\n", trip_summary.total_energy_consumed_wh);
	printf("Starting Battery State of CHarge: %f\n", trip_summary.starting_soc);
	printf("Ending Battery State of Charge: %f\n", trip_summary.ending_soc);



    return 0;
}