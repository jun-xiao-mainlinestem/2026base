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

// optical sensor for color sorting
optical ball_optical = optical(PORT8);
void color_sort()
{
  // Checks if the optical sensor is installed.
  if (ball_optical.installed()) {
    // If the color detected is red, intake the ball.
    if (ball_optical.color() == color::red) {
      print_controller_screen("red ball");
      in_take();
    } 
    // If the color detected is blue, score middle.
    else if (ball_optical.color() == color::blue) {
      print_controller_screen("blue ball");
      score_middle();
    } 
    // If no color is detected, default to intake.
    else {
      in_take();
      print_controller_screen("not red or blue");
    }
  } else {
    // If no optical sensor is installed, default to intake.
    in_take();
  }
} 

// (Optional) optical sensor for team color detection
optical team_optical = optical(PORT9);
bool team_is_red = true;
void setup_team_color(){
  if (team_optical.installed()) {
    // Sets the team color based on the optical sensor.
    if (team_optical.color() == color::blue) {
      team_is_red = false;
      print_controller_screen("team blue");
    } else {
      print_controller_screen("team red");
    }
    wait(1, seconds);
  } 
}