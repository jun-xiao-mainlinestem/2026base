using namespace vex;

// A global instance of the lift motor.
extern motor roller_bottom;
extern motor roller_middle;
extern motor roller_top;

void in_take();
void out_take();
void stop_rollers();
void score_middle();
void score_top();

/*
extern motor liftMotor;
// A global instance of the lift rotation sensor.
extern rotation liftRotation;
// A global instance of the arm distance sensor.
extern distance arm_distance;

// Lifts the arm to a predefined position.
void lift_arm();
// Releases the arm to a predefined position.
void release_arm();
// Toggles the lift between the up and down positions.
void toggle_lift();
// Detects if the arm is in the correct position.
void detect_arm();

*/
