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
void setChassisDefaults() {
  // Sets the heading of the chassis to the current heading of the inertial sensor.
  chassis.setHeading(inertial1.heading());

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
  chassis.setArcadeConstants(0.5, 0.5, 0.85);

  // Sets the exit conditions for the drive functions.
  // These conditions are used to determine when the drive function should exit.
  chassis.setDriveExitConditions(1, 200, 2000);
  // Sets the exit conditions for the turn functions.
  // These conditions are used to determine when the turn function should exit.
  chassis.setTurnExitConditions(1.5, 200, 1500);
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
  exitAuton();

  // This loop runs forever, controlling the robot during the driver control period.
  while (1) {
    if (!chassis.joystickTouched){
      if(controller(primary).Axis1.position() != 0 || controller(primary).Axis2.position() != 0 
        || controller(primary).Axis3.position() != 0 || controller(primary).Axis4.position() != 0) {
        chassis.joystickTouched = true;
      }
    }
    // This is the tank drive code.
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



