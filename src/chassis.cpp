// vex.h includes all of the headers for the VEX V5 library
#include "vex.h"

// All vex classes and functions are in the vex namespace
using namespace vex;

// A global instance of brain used for printing to the V5 Brain screen
brain Brain;

//drivetrain motor definitions. 
//If you only have 4  motors (or mecanum drive), assign leftMotor3, rightMotor3 to unused ports.
motor leftMotor1 = motor(PORT1, ratio6_1, true);
motor leftMotor2 = motor(PORT2, ratio6_1, true);
motor leftMotor3 = motor(PORT3, ratio18_1, false);

motor rightMotor1 = motor(PORT4, ratio6_1, false);
motor rightMotor2 = motor(PORT5, ratio6_1, false);
motor rightMotor3 = motor(PORT6, ratio18_1, true);

// inertial sensor for auton turning and heading
// If you do not have an inertial sensor, assign it to an unused port. Ignore the warning at the start of the program.
inertial inertial1 = inertial(PORT10);

// 0: arcade drive, 1: tank drive, 2: mecanum drive
int DRIVE_MODE = 0;


// ------------------------------------------------------------------------
//               Only change code below this line when necessary
// ------------------------------------------------------------------------

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

// Resets the chassis constants.
void resetChassis() {
  // Sets the heading of the chassis to the current heading of the inertial sensor.
  chassis.setHeading(inertial1.heading());
  // Stops the chassis.
  chassis.stop(coast);

  // Sets the drive constants for the chassis.
  // These constants are used to control the acceleration and deceleration of the chassis.
  chassis.setDrivePID(10, 1.5, 0, 10, 0);
  // Sets the turn constants for the chassis.
  // These constants are used to control the turning of the chassis.
  chassis.setTurnPID(10, 0.2, .015, 1.5, 7.5);
  // Sets the heading constants for the chassis.
  // These constants are used to control the heading adjustment of the chassis.
  chassis.setHeadingPID(6, .4, 1);

  // Sets the arcade drive constants for the chassis.
  // These constants are used to control the arcade drive of the chassis.
  chassis.setArcadeConstants(0.16, 0.5, 0.85);
}

// end game reminder will start at 85 seconds into the match
const int END_GAME_SECONDS = 85;
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

bool changeDriveMode(){
  // toggle tank or arcade drive mode if the button is pressed immediately after running the program
  if ((Brain.Timer.time(sec) < 5)) {
    controller(primary).rumble("-");
    DRIVE_MODE = (DRIVE_MODE == 0) ? 1 : 0;
    if (DRIVE_MODE == 1) {
      printControllerScreen("Drive Mode: Tank");
    } else {
      printControllerScreen("Drive Mode: Arcade");
    }
    return true;
  }
  return false;
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
    if(controller(primary).Axis1.position() != 0 || controller(primary).Axis2.position() != 0 
   || controller(primary).Axis3.position() != 0 || controller(primary).Axis4.position() != 0) {
      chassis.joystickTouched = true;
    }
    if (DRIVE_MODE == 1) chassis.controlTank(controller(primary).Axis3.position(), controller(primary).Axis2.position());
    // This is the arcade drive code.
    else if (DRIVE_MODE == 0)
      chassis.controlArcade(controller(primary).Axis2.position(), controller(primary).Axis4.position());
    else if (DRIVE_MODE == 2)
    {   
      // This is the mecanum drive code.
      chassis.controlMecanum(controller(primary).Axis4.position(), controller(primary).Axis3.position(), controller(primary).Axis2.position(), controller(primary).Axis1.position(), leftMotor1, leftMotor2, rightMotor1, rightMotor2);
    }
    
    // This wait prevents the loop from using too much CPU time.
    wait(20, msec);
   } 
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



