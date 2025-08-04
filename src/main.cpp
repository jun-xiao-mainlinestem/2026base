// vex.h includes all of the headers for the VEX V5 library
#include "vex.h"
#include "rgb-template/RemoteControl.h"

// All vex classes and functions are in the vex namespace
using namespace vex;
using namespace rgb;

// A global instance of competition
// This object is used to register callbacks for the autonomous and driver control periods.
competition Competition;

// ----------------------------------------------------------------------------
//                                Controller Callbacks
// ----------------------------------------------------------------------------

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
//                     No need to change code below this line
// ------------------------------------------------------------------------

// This function is called when the B button is pressed.
// It holds the drivetrain in place until the button is released.
void holdDrivetrain()
{
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


// Global flag autonTestMode indicating whether the robot is in autonomous test mode.
// When true, special button actions allow selection and testing of autonomous routines.
// This function is called when the Right button is pressed.
void enterTestMode()
{
  // Activate test mode if the button is pressed immediately after running the program
  if ((Brain.Timer.time(sec) < 5) && !autonTestMode) {
    controller(primary).rumble("-");
    autonTestMode = true;
    return;
  }
  // if in test mode, scroll through the auton menu
  if (autonTestMode)
  {
    currentAutonSelection = (currentAutonSelection + 1) % autonNum;
    showAutonMenu();
    return;
  }
  // otherwise, run other macro code
  chassis.driverControlDisabled = true;
  // TODO: Insert test code here. This is a placeholder for future actions triggered by Button Right.

  chassis.driverControlDisabled = false;

}

// This function is called when the Left button is pressed.
void changeDriveMode()
{
  // toggle tank or arcade drive mode if the button is pressed immediately after running the program
  if ((Brain.Timer.time(sec) < 5)) {
    controller(primary).rumble("-");
    DRIVE_MODE = (DRIVE_MODE == 0) ? 1 : 0;
    if (DRIVE_MODE == 1) {
      printControllerScreen("Drive Mode: Tank");
    } else {
      printControllerScreen("Drive Mode: Arcade");
    }
    return;
  }
  // if in test mode, scroll through the auton menu
  if (autonTestMode)
  {
    currentAutonSelection = (currentAutonSelection - 1) % autonNum;
    showAutonMenu();
    return;
  }

  // otherwise, run other macro code
  chassis.driverControlDisabled = true;
  // TODO: Insert test code here. This is a placeholder for future actions triggered by Button Left.

  chassis.driverControlDisabled = false;
}

// This function is called when the A button is pressed.
void testAutons()
{
  // If in test mode, run the selected autonomous routine for testing and displays the run time.
  if (autonTestMode)
  {
    chassis.driverControlDisabled = true;
    Brain.Timer.clear();

    runAutonItem(); 

    double t = Brain.Timer.time(sec);
    char timeMsg[30];
    sprintf(timeMsg, "run time: %.1f", t);
    printControllerScreen(timeMsg);
    chassis.driverControlDisabled = false;

    return;
  }

    // otherwise run macro code 
  chassis.driverControlDisabled = true;
  // TODO: Insert test code here. This is a placeholder for future actions triggered by Button A. 

  chassis.driverControlDisabled = false;
}



// Global remote control object
RemoteControl remoteControl;
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
  controller(primary).ButtonRight.pressed(enterTestMode);
  controller(primary).ButtonLeft.pressed(changeDriveMode);
  controller(primary).ButtonA.pressed(testAutons);
  controller(primary).ButtonB.pressed(holdDrivetrain);

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
