// vex.h includes all of the headers for the VEX V5 library
#include "vex.h"

// All vex classes and functions are in the vex namespace
using namespace vex;

// A global instance of brain used for printing to the V5 Brain screen
brain Brain;

// ----------------------------------------------------------------------------
//                                Drivetrain
// ----------------------------------------------------------------------------
// motor definitions
// The first parameter is the port number.
// The second parameter is the gear ratio.
// The third parameter is whether the motor is reversed or not.
motor leftMotor1 = motor(PORT1, ratio6_1, true);
motor leftMotor2 = motor(PORT2, ratio6_1, true);
motor leftMotor3 = motor(PORT3, ratio18_1, false);

motor rightMotor1 = motor(PORT4, ratio6_1, false);
motor rightMotor2 = motor(PORT5, ratio6_1, false);
motor rightMotor3 = motor(PORT6, ratio18_1, true);

// inertial sensor for turning and heading
inertial inertial1 = inertial(PORT10);

// 0: arcade drive, 1: tank drive, 2: mecanum drive
bool DRIVE_MODE = 0;

// constant definitions for driver control
// TURN_FACTOR reduces the sensitivity of the turn stick
const float TURN_FACTOR = 0.85;
// STEER_BIAS allows for a non-proportional steering response
const float STEER_BIAS = 0.5;

// end game reminder will start at 85 seconds into the match
const int END_GAME_SECONDS = 85;

// The Drive class is a wrapper for the VEX motor_group class.
// It provides a more intuitive interface for controlling a drivetrain.
Drive chassis(
  //Left Motors:
  motor_group(leftMotor1, leftMotor2, leftMotor3),
  //Right Motors:
  motor_group(rightMotor1, rightMotor2, rightMotor3),
  //Inertial Sensor:
  inertial1,
  //wheel diameter:
  2.75,
  //Gear ratio of motor to wheel: if your motor has an 36-tooth gear and your wheel has a 48-tooth gear, this value will be 0.75.
  0.75
);


// ------------------------------------------------------------------------
//                     No need to change code below this line
// ------------------------------------------------------------------------


// Resets the chassis to a known state.
// This function should be called before any autonomous routines.
void resetChassis() {
  // Sets the heading of the chassis to the current heading of the inertial sensor.
  chassis.setHeading(inertial1.heading());
  // Stops the chassis.
  chassis.stop(coast);

  // Sets the drive constants for the chassis.
  // These constants are used to control the acceleration and deceleration of the chassis.
  chassis.setDriveConstants(10, 1.5, 0, 10, 0);
  // Sets the heading constants for the chassis.
  // These constants are used to control the turning of the chassis.
  chassis.setHeadingConstants(6, .4, 1);
  // Sets the turn constants for the chassis.
  // These constants are used to control the turning of the chassis.
  chassis.setTurnConstants(10, 0.2, .015, 1.5, 7.5);

  // Sets the exit conditions for the drive functions.
  // These conditions are used to determine when the drive function should exit.
  chassis.setDriveExitConditions(1, 200, 2000);
  // Sets the exit conditions for the turn functions.
  // These conditions are used to determine when the turn function should exit.
  chassis.setTurnExitConditions(1.5, 200, 1500);
}


// This function is a thread that runs in the background to remind the driver of the end game.
int endgameTimer() {
  // Clears the brain timer.
  Brain.Timer.clear();
  // Waits until the end game starts.
  while (Brain.Timer.time(sec) < END_GAME_SECONDS) {
    wait(200, msec);
  }
  // Prints a message to the controller screen.
  printControllerScreen("end game");
  // Rumbles the controller.
  controller(primary).rumble("-");
  // Checks the motors every 60 seconds.
  while(true)
  {
    wait(60, seconds);
    checkMotors(NUMBER_OF_MOTORS);
  }
  return 1;
}

// This is the user control function.
// It is called when the driver control period starts.
void usercontrol(void) {
  // Exits the autonomous menu.
  exitAutonMenu = true;
  // Resets the chassis.
  resetChassis();
  // Starts the end game timer thread.
  thread endgameTimer_thread = thread(endgameTimer);

  // This loop runs forever, controlling the robot during the driver control period.
  while (1) {
    // This is the tank drive code.
    if (DRIVE_MODE == 1) chassis.controlTank(controller(primary).Axis3.position(), controller(primary).Axis2.position());
    // This is the arcade drive code.
    else if (DRIVE_MODE == 0)
      chassis.controlArcade(controller(primary).Axis2.position(), controller(primary).Axis4.position() * TURN_FACTOR, STEER_BIAS);
    else if (DRIVE_MODE == 2)
    {   
      // This is the mecanum drive code.
      chassis.controlMecanum(controller(primary).Axis4.position(), controller(primary).Axis3.position(), controller(primary).Axis2.position(), controller(primary).Axis1.position(), leftMotor1, leftMotor2, rightMotor1, rightMotor2);
    }
   }
    // This wait prevents the loop from using too much CPU time.
    wait(20, msec); 
}

// This function sets up the gyro.
bool setupGyro() {
  // Waits until the inertial sensor is calibrated.
  while (inertial1.isCalibrating()) {
    wait(25, msec);
  }
  // Rumbles the controller to indicate that the gyro is calibrated.
  controller(primary).rumble("-");
  // If the inertial sensor is not installed, print an error message to the controller screen.
  if (!inertial1.installed()) {
    printControllerScreen("inertial sensor failure");
    controller(primary).rumble("----");
    wait(2, seconds);
    return false;  
  }
  return true;
}


// This function is called before the autonomous period starts.
void pre_auton() {
  // Sets up the gyro.
  bool gyroSetupSuccess = setupGyro();
  // Sets up the team color.
  setupTeamColor();

  bool motorsSetupSuccess = true;
  // Checks the motors.
  motorsSetupSuccess = checkMotors(NUMBER_OF_MOTORS);
  // Resets the chassis.
  resetChassis();
  // Shows the autonomous menu.
  if(gyroSetupSuccess && motorsSetupSuccess) showAutonMenu();
}

// This function returns true when the joystick is touched.
bool joystickTouched() {
  float d = fabs(chassis.getLeftPositionIn()) + fabs(chassis.getRightPositionIn());
  if (d > 1) {
    return true;
  }
  else {
    return false;
  }
}

