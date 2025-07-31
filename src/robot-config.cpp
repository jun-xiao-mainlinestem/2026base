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

// optical sensor for color sorting
optical ballOptical = optical(PORT14);

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

void colorSort()
{
  // Checks if the optical sensor is installed.
  if (ballOptical.installed()) {    
    // Get the detected ball color
    color detectedColor = ballOptical.color();    
    if (detectedColor == color::red) {
      printControllerScreen("red ball");
    } 
    if (detectedColor == color::blue) {
      printControllerScreen("blue ball");
    }     

    // If the ball color matches the team color, intake it
    if ((teamIsRed && detectedColor == color::red) || (!teamIsRed && detectedColor == color::blue)) {
      inTake();
    } 
    // If the ball color doesn't match team color, eject it by score it
    else if (detectedColor == color::red || detectedColor == color::blue) {
      scoreLong();
    } 
    // If no red or blue color is detected, default to intake
    else {
      inTake();
    }
  } else {
    // If no optical sensor is installed, default to intake
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
      printControllerScreen("team blue");
    } else {
      printControllerScreen("team red");
    }
    wait(1, seconds);
  } 
}