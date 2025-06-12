#include "vex.h"

using namespace vex;

// A global instance of brain used for printing to the V5 Brain screen
brain Brain;

// motor definitions
motor leftMotor1 = motor(PORT1, ratio6_1, true);
motor leftMotor2 = motor(PORT2, ratio6_1, true);
motor leftMotor3 = motor(PORT3, ratio18_1, false);

motor rightMotor1 = motor(PORT4, ratio6_1, false);
motor rightMotor2 = motor(PORT5, ratio6_1, false);
motor rightMotor3 = motor(PORT6, ratio18_1, true);

inertial inertial1 = inertial(PORT9);

// 0: double arcade   1: tank
bool DRIVE_TANK_MODE = false;

// total number of motors, including drivetrain
const int NUMBER_OF_MOTORS = 6;

// end game reminder
const int END_GAME_SECONDS = 75;

// constant definitions for driver control
const float TURN_FACTOR = 0.85;
const float STEER_BIAS = 0.5;

Drive chassis(
  //Left Motors:
  motor_group(leftMotor1, leftMotor2, leftMotor3),
  //Right Motors:
  motor_group(rightMotor1, rightMotor2, rightMotor3),
  inertial1,
  //wheel diameter:
  2.75,
  //Gear ratio of motor to wheel: if your motor has an 84-tooth gear and your wheel has a 60-tooth gear, this value will be 1.4.
  0.75
);

void reset_chassis() {
  chassis.set_heading(inertial1.heading());
  chassis.stop(coast);

  chassis.set_drive_constants(10, 1.5, 0, 10, 0);
  chassis.set_heading_constants(6, .4, 1);
  chassis.set_turn_constants(10, 0.2, .015, 1.5, 7.5);

  chassis.set_drive_exit_conditions(1, 200, 2000);
  chassis.set_turn_exit_conditions(1.5, 200, 1500);
}

int endgame_timer() {
  Brain.Timer.clear();
  while (Brain.Timer.time(sec) < END_GAME_SECONDS) {
    wait(200, msec);
  }
  controller(primary).Screen.print("end game ...");
  controller(primary).rumble("-");
  while(true)
  {
    wait(60, seconds);
    check_motors(NUMBER_OF_MOTORS);
  }
  return 1;
}

bool exit_auton_menu = false;
void usercontrol(void) {
  exit_auton_menu = true;
  reset_chassis();
  thread endgame_timer_thread = thread(endgame_timer);

  // do other things before driver control starts
  while (1) {
    if (DRIVE_TANK_MODE) chassis.control_tank(controller(primary).Axis3.position(), controller(primary).Axis2.position());
    else {
      if (abs(controller(primary).Axis4.position()) < 100)
        chassis.control_arcade(controller(primary).Axis2.position(), controller(primary).Axis4.position() * TURN_FACTOR, STEER_BIAS);
      else
        chassis.control_arcade(controller(primary).Axis2.position(), controller(primary).Axis4.position(), STEER_BIAS);
    }
    wait(20, msec); 
  }
}

void autonomous(void) {
  exit_auton_menu = true;
  run_auton_item();
}

void print_menu_item(char const * txt[]) {
  if (current_auton_selection < 0) {
    controller(primary).Screen.print("custom test");
    return;
  }    
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(3, 1);
  Brain.Screen.print("%s", txt[current_auton_selection]);
  controller(primary).Screen.print("%s", txt[current_auton_selection]);
}

void print_menu(char const * txt[]) {
  Brain.Screen.setFont(mono30);
  print_menu_item(txt);

  while (!exit_auton_menu) {
    if (Brain.Screen.pressing()) {
      while (Brain.Screen.pressing()) {
        //wait until finger lifted up from the screen
        wait(20, msec);
      }
      current_auton_selection = (current_auton_selection + 1) % auton_num;
      print_menu_item(txt);
      controller(primary).rumble(".");
    }
    wait(50, msec);
  }
  Brain.Screen.setFont(mono20);
}

void setup_gyro() {
  while (inertial1.isCalibrating()) {
    wait(25, msec);
  }
  controller(primary).rumble("-");
  if (!inertial1.installed()) {
    controller(primary).Screen.print("inertial sensor failure");
    controller(primary).rumble("----");
  }
}

void pre_auton() {
  setup_gyro();
  check_motors(NUMBER_OF_MOTORS);
  reset_chassis();
  show_auton_menu();
}
