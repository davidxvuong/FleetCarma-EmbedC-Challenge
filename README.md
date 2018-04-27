# FleetCarma Embedded C Challenge

## Summary of Challenge

Write a library in C which will process a set of signals generated by an electric vehicle and compile a trip summary for the vehicle in motion.

The input signals are of the type `vehicle_signal_t`:

```
typedef struct {
    vehicle_signal_type_t signal_type;
    uint64_t unix_timestamp_milliseconds;
    float value;
}vehicle_signal_t;
```

The trip summary structure are of the type `trip_event_summary_t`:

```
typedef struct {
    uint32_t start_time_since_unix_epoch_seconds;
    uint32_t duration_seconds;
    uint32_t distance_travelled_meters;
    uint32_t total_energy_consumed_wh;
    float starting_soc;
    float ending_soc;
}trip_event_summary_t;
```

For more information, feel free to visit the [FleetCarma's EmbedC Challenge](https://github.com/FleetCarma/embedC-Challenge) webpage.

## Highlights of Implementation

Part of the challenge was to determine the vehicle's distance travelled and the net energy consumed by the vehicle. As a disclaimer, I am not that good at physics, but I still remember some key parts taught in my 1st year physics and calculus courses (specifically, [MATH 117](https://uwflow.com/course/math117), [ECE 105](https://uwflow.com/course/ece105), and [ECE 106](https://uwflow.com/course/ece106)).

Recall that the formula for determining speed is the derivative of distance with respect to time, and and the formula for determining the power is the derivative of energy with respect to time.

<p align=center>
	<img src="https://latex.codecogs.com/gif.latex?v=\frac{\mathrm{d}&space;s}{\mathrm{d}&space;t}" title="v=\frac{\mathrm{d} s}{\mathrm{d} t}" />
</p>
<br>
<p align=center>
	<img src="https://latex.codecogs.com/gif.latex?P=\frac{\mathrm{d}&space;W}{\mathrm{d}&space;t}" title="P=\frac{\mathrm{d} W}{\mathrm{d} t}" />
</p>

Reversing these derivatives will create the formula for distance and energy. The formula for distance is the integral of speed with respect to time, and the formula for energy is the integral of pwoer with respect to time.

<p align=center>
	<img src="https://latex.codecogs.com/gif.latex?W=\int&space;Pdt" title="W=\int Pdt" />
</p>
<br>
<p align=center>
	<img src="https://latex.codecogs.com/gif.latex?d=\int&space;vdt" title="d=\int vdt" />
</p>

Our C code will need to perform integration in order to get the values we need. According to the coding challenge, it doesn't matter which integration method we use. So I figured that the simplest was to use the [trapezoidal rule](http://tutorial.math.lamar.edu/Classes/CalcII/ApproximatingDefIntegrals.aspx). This states that the integral of any function can be approximated by the area of several smaller regions approximated by a trapezoid, where the slant of the trapezoid represents the curve on the function to be integrated. 


<p align=center>
	<img src="http://tutorial.math.lamar.edu/Classes/CalcII/ApproximatingDefIntegrals_files/image002.gif" />
</p>

The derivation of the approximation is shown in the link above. Below provides the final result of the approximation.

<p align=center>
	<img src="http://tutorial.math.lamar.edu/Classes/CalcII/ApproximatingDefIntegrals_files/eq0018MP.gif" />
</p>

Graph credits goes to Paul's Online Math Notes. 

The following code snippet is the implementation of the integration method:

```
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
```

Note: The speed and power function in this case are constants, so technically we didn't need to pass in a function pointer to call that function. However, I wanted to treat this integration function as a black box so that in a workplace environment, this function can be reused elsewhere. Also, the `constant` parameter in the function is used to pass in the coefficent in the function (in this case, the function is a polynomial). If there are multiple coefficients, then this will need to be modified such that we pass in multiple coefficents (perhaps an array of coefficents?).