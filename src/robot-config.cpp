// vex.h includes all of the headers for the VEX V5 library
#include "vex.h"

// All vex classes and functions are in the vex namespace
using namespace vex;

// ----------------------------------------------------------------------------
//                                Robot Configuration
// ----------------------------------------------------------------------------

// motor and sensor definitions
motor roller_bottom = motor(PORT20, ratio18_1, true);
motor roller_middle = motor(PORT20, ratio18_1, true);
motor roller_top = motor(PORT20, ratio6_1, true);

void in_take() {
  roller_bottom.spin(forward, 12, volt);
  roller_middle.spin(forward, 12, volt);
  roller_top.stop(coast);
}

void out_take() {
  roller_bottom.spin(forward, -12, volt);
  roller_middle.spin(forward, -12, volt);
  roller_top.stop(coast);
}

void stop_rollers() {
  // Stops the roller motors.
  roller_bottom.stop(coast);
  roller_middle.stop(coast);
  roller_top.stop(coast);
}

void score_middle() {
  roller_bottom.spin(forward, 12, volt);
  roller_middle.spin(forward, -12, volt);
  roller_top.spin(forward, -12, volt);
}

void score_top() {
  roller_bottom.spin(forward, -12, volt);
  roller_middle.spin(forward, -12, volt);
  roller_top.spin(forward, 12, volt);
}

/*
motor liftMotor = motor(PORT7, ratio36_1, false);
rotation liftRotation = rotation(PORT10, true);
distance arm_distance = distance(PORT11);

// This variable is used to track the state of the lift.
bool lift_is_up = true;

// This function is called when the arm distance sensor detects an object.
// It lifts the arm if the object is within 1 inch and the lift is up.
void detect_arm(){
  if (arm_distance.objectDistance(inches) < 1 && lift_is_up){
    lift_arm();
  }
}

// This function rotates the lift to a specified angle using a PID controller.
void rotate_lift_to(float angle)
{
  // The PID controller is used to control the lift motor.
  // The first parameter is the target angle.
  // The next three parameters are the PID constants (kp, ki, kd).
  // The next two parameters are the integral and derivative limits.
  // The last two parameters are the minimum and maximum output values.
  PID liftPID(angle - liftRotation.position(deg), 0.4, 0.2, 0, 15, 3, 200, 1000);
  // This loop runs until the PID controller is done.
  while (liftPID.is_done() == false) {
    // The error is the difference between the target angle and the current angle.
    float error = (angle - liftRotation.position(deg));
    // The output is the value that is sent to the motor.
    float output = liftPID.compute(error);
    // The output is thresholded to prevent the motor from moving too fast.
    output = threshold(output, -10, 10);
    // The motor is spun at the calculated output.
    liftMotor.spin(forward, output, volt);
    // This wait prevents the loop from using too much CPU time.
    wait(10, msec);
  }
  // The motor is stopped when the PID controller is done.
  liftMotor.stop(hold);
}

// This function lifts the arm to a predefined angle.
void lift_arm(){
  rotate_lift_to(240);
}

// This function releases the arm to a predefined angle.
void release_arm()
{
  rotate_lift_to(40);
}

// This function toggles the lift between the up and down positions.
void toggle_lift() {
  if (lift_is_up == true) {
   lift_arm();
   lift_is_up = false;
  }
  else {
   release_arm();
   lift_is_up = true;
  }
  // This wait prevents the function from being called too quickly.
  wait (200, msec);
} */
