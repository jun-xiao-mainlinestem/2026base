// vex.h includes all of the headers for the VEX V5 library
#include "vex.h"

// All vex classes and functions are in the vex namespace
using namespace vex;

// ----------------------------------------------------------------------------
//                                Robot Configuration
// ----------------------------------------------------------------------------

// motor and sensor definitions
motor rollerBottom = motor(PORT11, ratio18_1, true);
motor rollerMiddle = motor(PORT12, ratio18_1, true);
motor rollerTop = motor(PORT13, ratio6_1, true);

// total number of motors, including drivetrain
const int NUMBER_OF_MOTORS = 9;

void inTake() {
  ballOptical.setLightPower(100, percent);
  rollerBottom.spin(forward, 12, volt);
  rollerMiddle.spin(forward, 12, volt);
  rollerTop.stop(coast);
}

void outTake() {
  rollerBottom.spin(forward, -12, volt);
  rollerMiddle.spin(forward, -12, volt);
  rollerTop.stop(coast);
}

void stopRollers() {
  // Stops the roller motors.
  rollerBottom.stop(brake);
  rollerMiddle.stop(brake);
  rollerTop.stop(brake);
  ballOptical.setLightPower(0, percent);

}

void scoreMiddle() {
  rollerBottom.spin(forward, 6, volt);
  rollerMiddle.spin(forward, -12, volt);
  rollerTop.spin(forward, -6, volt);
}

void scoreLong() {
  rollerBottom.spin(forward, 12, volt);
  rollerMiddle.spin(forward, -12, volt);
  rollerTop.spin(forward, 12, volt);
}

// optical sensor for color sorting
optical ballOptical = optical(PORT14);
void colorSort()
{
  // Checks if the optical sensor is installed.
  if (ballOptical.installed()) {
    ballOptical.setLightPower(100, percent);
    // If the color detected is red, intake the ball.
    if (ballOptical.color() == color::red) {
      print_controller_screen("red ball");
      inTake();
    } 
    // If the color detected is blue, score middle.
    else if (ballOptical.color() == color::blue) {
      print_controller_screen("blue ball");
      scoreMiddle();
    } 
    // If no color is detected, default to intake.
    else {
      inTake();
    //  print_controller_screen("not red or blue");
    }
  } else {
    // If no optical sensor is installed, default to intake.
    inTake();
  }
} 

// (Optional) optical sensor for team color detection
optical teamOptical = optical(PORT8);
bool teamIsRed = true;
void setupTeamColor(){
  if (teamOptical.installed()) {
    // Sets the team color based on the optical sensor.
    if (teamOptical.color() == color::blue) {
      teamIsRed = false;
      print_controller_screen("team blue");
    } else {
      print_controller_screen("team red");
    }
    wait(1, seconds);
  } 
}