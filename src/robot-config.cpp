#include "vex.h"
using namespace vex;
brain Brain;
controller controller1 = controller(primary);

// ------------------------------------------------------------------------
//              Drivetrain definition
// ------------------------------------------------------------------------
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

// 0: double arcade drive, 1: single aracde, 2: tank drive, 3: mecanum drive
// -1: disable drive
int DRIVE_MODE = 0;


// ------------------------------------------------------------------------
//        Other subsystems: motors, sensors and helper functions definition
// ------------------------------------------------------------------------
motor rollerBottom = motor(PORT11, ratio18_1, false);
motor rollerMiddle = motor(PORT12, ratio18_1, true);
motor rollerSort = motor(PORT14, ratio18_1, true);
motor rollerTop = motor(PORT13, ratio6_1, true);



// total number of motors, including drivetrain
const int NUMBER_OF_MOTORS = 9;

// (optional) intall an optical sensor right next to the license plate
// teamIsRed variable will be automatically set to false if blue license plate is detected
optical teamOptical = optical(PORT8);
bool teamIsRed = true;

// distance sensor at the front
distance frontDistance = distance(PORT16);
// optical sensor for color sorting
optical ballOptical = optical(PORT15);

// match load piston
digital_out matchLoadPiston = digital_out(Brain.ThreeWirePort.B);
digital_out hornPiston = digital_out(Brain.ThreeWirePort.C);

bool matchLoadOn = false;
bool toggleMatchLoad(){
  matchLoadOn = !matchLoadOn;
  matchLoadPiston.set(matchLoadOn);
  return matchLoadOn;
}

bool hornOn = false;
bool toggleHorn(){
  hornOn = !hornOn;
  hornPiston.set(hornOn);
  return hornOn;
}

void rollerTest(){
}

void intake() {
  rollerBottom.spin(forward, 12, volt);
  rollerMiddle.spin(forward, 12, volt);
  rollerSort.spin(forward, 12, volt);
  rollerTop.stop(coast);
}

void outTake() {
  rollerBottom.spin(forward, -12, volt);
  rollerMiddle.spin(forward, -12, volt);
  rollerSort.stop(hold);
  rollerTop.stop(coast);
}

void stopRollers() {
  // Stops the roller motors.
  rollerBottom.stop(brake);
  rollerMiddle.stop(brake);
  rollerSort.stop(brake);
  rollerTop.stop(brake);
  if (ballOptical.installed()) ballOptical.setLightPower(0, percent);
}

void scoreMiddle() {
  rollerBottom.spin(forward, 12, volt);
  rollerMiddle.spin(forward, -12, volt);
  rollerSort.spin(forward, 12, volt);
  rollerTop.stop(coast);
}

void scoreLong() {
  rollerBottom.spin(forward, 12, volt);
  rollerMiddle.spin(forward, 12, volt);
  rollerSort.spin(forward, 12, volt);
  rollerTop.spin(forward, 12, volt);
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
      rollerSort.spin(forward, -12, volt);
      wait(0.5, sec); // Wait for the rollers to finish ejecting the ball
      intake();
    } 
  }
} 

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
  chassis.setHeading(chassis.gyro.heading());

  // Sets the drive PID constants for the chassis.
  // These constants are used to control the acceleration and deceleration of the chassis.
  chassis.setDrivePID(10, 1.5, 0, 10, 0);
  // Sets the turn PID constants for the chassis.
  // These constants are used to control the turning of the chassis.
  chassis.setTurnPID(10, 0.2, .015, 1.5, 7.5);
  // Sets the heading PID constants for the chassis.
  // These constants are used to control the heading adjustment of the chassis.
  chassis.setHeadingPID(6, .4, 1);
  // Sets the exit conditions for the drive functions.
  // These conditions are used to determine when the drive function should exit.
  chassis.setDriveExitConditions(1, 200, 2000);
  // Sets the exit conditions for the turn functions.
  // These conditions are used to determine when the turn function should exit.
  chassis.setTurnExitConditions(1.5, 200, 1500);

  // Sets the arcade drive constants for the chassis.
  // These constants are used to control the arcade drive of the chassis.
  chassis.setArcadeConstants(0.5, 0.5, 0.85);
}

void changeDriveMode(){
  controller1.rumble("-");
  DRIVE_MODE = (DRIVE_MODE +1)%3;
    switch (DRIVE_MODE) {
    case 0:
      printControllerScreen("Double Arcade");
      break;
    case 1:
      printControllerScreen("Single Arcade");
      break;
    case 2:
      printControllerScreen("Tank Drive");
      break;
    case 3:
      printControllerScreen("Mecanum Drive");
      break;
    }
}

// This is the user control function.
// It is called when the driver control period starts.
void usercontrol(void) {
  // Exits the autonomous menu.
  exitAuton();

  // This loop runs forever, controlling the robot during the driver control period.
  while (1) {
    if (!chassis.joystickTouched){
      if(controller1.Axis1.position() != 0 || controller1.Axis2.position() != 0 
        || controller1.Axis3.position() != 0 || controller1.Axis4.position() != 0) {
        chassis.joystickTouched = true;
      }
    }
    switch (DRIVE_MODE) {
    case 0: // double arcade
      chassis.controlArcade(controller1.Axis2.position(), controller1.Axis4.position());
      break;
    case 1: // single arcade
      chassis.controlArcade(controller1.Axis3.position(), controller1.Axis4.position());
      break;
    case 2: // tank drive
      chassis.controlTank(controller1.Axis3.position(), controller1.Axis2.position());      break;
    case 3: // mecanum drive
      chassis.controlMecanum(controller1.Axis4.position(), controller1.Axis3.position(), controller1.Axis2.position(), controller1.Axis1.position(), leftMotor1, leftMotor2, rightMotor1, rightMotor2);
      break;
    }

    // This wait prevents the loop from using too much CPU time.
    wait(20, msec);
   } 
}

