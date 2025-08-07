// vex.h includes all of the headers for the VEX V5 library
#include "vex.h"

// All vex classes and functions are in the vex namespace
using namespace vex;

// motor and sensor definitions
motor rollerBottom = motor(PORT11, ratio18_1, true);
motor rollerMiddle = motor(PORT12, ratio18_1, true);
motor rollerTop = motor(PORT13, ratio6_1, true);


// total number of motors, including drivetrain
const int NUMBER_OF_MOTORS = 9;

// optical sensor for color sorting
optical ballOptical = optical(PORT14);

void inTake() {
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
  if (ballOptical.installed()) ballOptical.setLightPower(0, percent);
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

void ejectBalls() {
  scoreMiddle();
}

void colorSort()
{
  // Checks if the optical sensor is installed.
  if (ballOptical.installed()) {    
    ballOptical.setLightPower(100, percent);
    // Get the detected ball color
    color detectedColor = ballOptical.color();    
    if (detectedColor == color::red) {
      printControllerScreen("red ball");
    } 
    if (detectedColor == color::blue) {
      printControllerScreen("blue ball");
    }     

    // If the ball color does not match the team color, eject it
    if ((teamIsRed && detectedColor == color::blue) || (!teamIsRed && detectedColor == color::red)) {
      ejectBalls();
      wait(0.5, sec); // Wait for the rollers to finish ejecting the ball
    } 
  }
  inTake();
} 

// (Optional) optical sensor for team color detection
optical teamOptical = optical(PORT8);
bool teamIsRed = true;
void setupTeamColor(){
  if (teamOptical.installed()) {
    // Sets the team color based on the optical sensor.
    if (teamOptical.color() == color::blue) {
      teamIsRed = false;
      printControllerScreen("team blue");
    } else {
      printControllerScreen("team red");
    }
    wait(1, seconds);
  } 
}

void collisionDetected(axisType axis, double raw_x, double raw_y, double raw_z ) {
  chassis.stop(hold);
}

void setCollisionCallback()
{
  inertial1.collision(collisionDetected);
}
