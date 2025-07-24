// vex.h includes all of the headers for the VEX V5 library
#include "vex.h"

// All vex classes and functions are in the vex namespace
using namespace vex;

// ----------------------------------------------------------------------------
//                                Robot Configuration
// ----------------------------------------------------------------------------

// motor and sensor definitions
motor roller_bottom = motor(PORT11, ratio18_1, true);
motor roller_middle = motor(PORT12, ratio18_1, true);
motor roller_top = motor(PORT13, ratio6_1, true);

// total number of motors, including drivetrain
const int NUMBER_OF_MOTORS = 9;

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
  roller_bottom.stop(brake);
  roller_middle.stop(brake);
  roller_top.stop(brake);
}

void score_middle() {
  roller_bottom.spin(forward, 6, volt);
  roller_middle.spin(forward, -12, volt);
  roller_top.spin(forward, -6, volt);
}

void score_long() {
  roller_bottom.spin(forward, 12, volt);
  roller_middle.spin(forward, -12, volt);
  roller_top.spin(forward, 12, volt);
}

