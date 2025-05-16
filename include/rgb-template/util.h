float reduce_0_to_360(float angle);

float reduce_negative_180_to_180(float angle);

float to_deg(float angle_rad);

float threshold(float input, float min, float max);

float to_volt(float percent);

float deadband(float input, float width);

bool check_motors(int motor_count, int temperature_limit = 45);