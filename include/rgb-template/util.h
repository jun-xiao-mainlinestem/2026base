// Reduces an angle to the range [0, 360).
float reduce_0_to_360(float angle);

// Reduces an angle to the range [-180, 180).
float reduce_negative_180_to_180(float angle);

// Converts an angle from radians to degrees.
float to_deg(float angle_rad);

// Clamps a value between a minimum and maximum value.
float threshold(float input, float min, float max);

// Converts a percentage to a voltage.
float to_volt(float percent);

// Creates a dead zone for a joystick axis.
float deadband(float input, float width);

// Checks if all motors are connected and not overheating.
bool check_motors(int motor_count, int temperature_limit = 45);