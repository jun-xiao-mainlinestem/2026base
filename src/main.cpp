// vex.h includes all of the headers for the VEX V5 library
#include "vex.h"

// All vex classes and functions are in the vex namespace
using namespace vex;

// A global instance of competition
// This object is used to register callbacks for the autonomous and driver control periods.
competition Competition;

bool macroMode = false;

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

bool matchLoaderDown = false;
void buttonL2Action() {
  if(controller(primary).ButtonL1.pressing()) 
  {
    matchLoaderDown = !matchLoaderDown;
    // toggle the matchloader position
    return;
  }
  outTake();
  // Wait until the button is released to stop the rollers.
  while(controller(primary).ButtonL2.pressing()) {
    wait (20, msec);
  }
  stopRollers();
}

void buttonR1Action() {
  chassis.stop(hold);
  if(controller(primary).ButtonR2.pressing()) scoreMiddle();
  else scoreLong();

  // Wait until the button is released to stop the rollers.
  while(controller(primary).ButtonR1.pressing()) {
    wait (20, msec);
  }
  chassis.stop(coast);
  stopRollers();
}

void setupButtonMapping() {
  controller(primary).ButtonL1.pressed(buttonL1Action);
  controller(primary).ButtonL2.pressed(buttonL2Action);
  controller(primary).ButtonR1.pressed(buttonR1Action);
}


// 
// ------------------------------------------------------------------------
//               Only change code below this line when necessary
// ------------------------------------------------------------------------

// This function is called when the Right button is pressed.
void buttonRightAction()
{
  if (enterTestMode()) return;
  if (nextAutonMenu()) return;
}

void buttonLeftAction()
{
  if (changeDriveMode()) return;
  if (prevAutonMenu()) return;
}

void buttonDownAction()
{
  if (nextAutonStep()) return;
  if (macroMode) return; // prevent re-entry

  if (fabs(chassis.getHeading()) - 180 > 3) {
    chassis.driveWithVoltage(-6, -6);
    wait(100, msec);
    chassis.turnToHeading(180);
  }
  if (!controller(primary).ButtonDown.pressing()) return;
  
  macroMode = true;
  // This is a placeholder for future actions triggered by Button Down.
  // safty check to prevent running the code if the distance reading is not valid.
  // Matchload balls when the Button Down is pressed and hold.
  // end matchloading and turn around to score when the Button Down is released.
  macroMode = false;
  chassis.stop(coast);
}

void buttonUpAction()
{
  if (prevAutonStep()) return;
}


void buttonAAction()
{
  if (runAutonTest()) return;
}

void buttonR2Action()
{
  // brakes the drivetrain until the button is released.
  chassis.stop(hold);
  controller(primary).rumble(".");
  waitUntil(!controller(primary).ButtonR2.pressing());
  chassis.checkStatus();
  chassis.stop(coast);
}

// remote control flag
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
  controller(primary).ButtonR2.pressed(buttonR2Action);

  //controller(primary).ButtonX.pressed(aiAction);

  // Set up other button mapping for the controller.
  setupButtonMapping();

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    if (REMOTE_CONTROL_MODE) {
     chassis.pollRemoteCommand();
    }
    wait(100, msec);
  }
}
