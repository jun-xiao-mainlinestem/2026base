// vex.h includes all of the headers for the VEX V5 library
#include "vex.h"
#include "rgb-template/RemoteControl.h"

// All vex classes and functions are in the vex namespace
using namespace vex;
using namespace rgb;

// A global instance of competition
// This object is used to register callbacks for the autonomous and driver control periods.
competition Competition;

// This function is called when the L1 button is pressed.
// It performs color sorting.
void buttonL1Action() {
  inTake();
  
  // Wait until the button is released to stop the rollers.
  while(controller(primary).ButtonL1.pressing()) {
    colorSort();
    wait (20, msec);
  }
  stopRollers();
}

void buttonL2Action() {
  outTake();
  // Wait until the button is released to stop the rollers.
  while(controller(primary).ButtonL2.pressing()) {
    wait (20, msec);
  }
  stopRollers();
}

void buttonR1Action() {
  chassis.stop(hold);
  scoreLong();
  // Wait until the button is released to stop the rollers.
  while(controller(primary).ButtonR1.pressing()) {
    wait (20, msec);
  }
  chassis.stop(coast);
  stopRollers();
}

// This function is called when the R2 button is pressed.
void buttonR2Action() {
  chassis.stop(hold);
  scoreMiddle();
  while(controller(primary).ButtonR2.pressing()) {
    wait (20, msec);
  }  
  chassis.stop(coast);
  stopRollers();
}

void setupButtonMapping() {
  controller(primary).ButtonL1.pressed(buttonL1Action);
  controller(primary).ButtonL2.pressed(buttonL2Action);
  controller(primary).ButtonR1.pressed(buttonR1Action);
  controller(primary).ButtonR2.pressed(buttonR2Action);
}


// ------------------------------------------------------------------------
//               Only change code below this line when necessary
// ------------------------------------------------------------------------

// This function is called when the Right button is pressed.
void buttonRightAction()
{
  if (enterTestMode()) return;
  if (nextAutonMenu()) return;
 
  // otherwise, run other macro code
  chassis.driverControlDisabled = true;
  // TODO: Insert test code here. This is a placeholder for future actions triggered by Button Right.

  chassis.driverControlDisabled = false;
}

void buttonLeftAction()
{
  if (prevAutonMenu()) return;
 
  // otherwise, run other macro code
  chassis.driverControlDisabled = true;
  // TODO: Insert test code here. This is a placeholder for future actions triggered by Button Right.

  chassis.driverControlDisabled = false;
}

void buttonDownAction()
{
  if (nextAutonStep()) return;

    // otherwise run macro code 
  chassis.driverControlDisabled = true;
  // TODO: Insert test code here. This is a placeholder for future actions triggered by Button A. 

  chassis.driverControlDisabled = false;
}

void buttonUpAction()
{
  if (prevAutonStep()) return;

    // otherwise run macro code 
  chassis.driverControlDisabled = true;
  // TODO: Insert test code here. This is a placeholder for future actions triggered by Button A. 

  chassis.driverControlDisabled = false;
}


void buttonAAction()
{
  if (runAutonTest()) return;

    // otherwise run macro code 
  chassis.driverControlDisabled = true;
  // TODO: Insert test code here. This is a placeholder for future actions triggered by Button A. 

  chassis.driverControlDisabled = false;
}

void buttonBAction()
{
  if (changeDriveMode()) return;

  // otherwise brakes the drivetrain until the button is released.
  chassis.drivetrainNeedsStopped = true;
  double distanceTraveled = (chassis.getLeftPositionIn() + chassis.getRightPositionIn()) / 2.0;
  chassis.stop(hold);
  controller(primary).rumble(".");
  wait(0.5, sec);
  // Display heading and the distance traveled previously on the controller screen.
  float h = chassis.getHeading();
  char statusMsg[50];
  sprintf(statusMsg, "heading: %.1f, distance: %.1f", h, distanceTraveled);
  printControllerScreen(statusMsg);

  waitUntil(!controller(primary).ButtonB.pressing());
  chassis.stop(coast);
}


// Global remote control object
SampleRemoteControl remoteControl;
bool REMOTE_CONTROL_MODE = true;
// ----------------------------------------------------------------------------
//                                Main
// ----------------------------------------------------------------------------
//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Register the autonomous and driver control functions.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Register the default controller button callbacks.
  controller(primary).ButtonRight.pressed(buttonRightAction);
  controller(primary).ButtonLeft.pressed(buttonLeftAction);
  controller(primary).ButtonDown.pressed(buttonDownAction);
  controller(primary).ButtonUp.pressed(buttonUpAction);
  controller(primary).ButtonA.pressed(buttonAAction);
  controller(primary).ButtonB.pressed(buttonBAction);

  // Set up the button mapping for the controller.
  setupButtonMapping();

  // Run the pre-autonomous function.
  pre_auton();

  // optional: set up the team color
  setupTeamColor();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    if (REMOTE_CONTROL_MODE) {
      remoteControl.poll();
    }
    wait(100, msec);
  }
}
