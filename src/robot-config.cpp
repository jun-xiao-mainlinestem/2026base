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

// constant definitions for driving control
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

int drive_mode = 0;
// 0: arcade   1: tank
void usercontrol(void) {
  // task end_game_reminder(endgame_timer);
  exit_auton_menu = true;
  reset_chassis();
  while (1) {
    if (drive_mode == 1) chassis.control_tank(controller(primary).Axis3.position(), controller(primary).Axis2.position());
    if (drive_mode == 0) {
      if (abs(controller(primary).Axis4.position()) < 100)
        chassis.control_arcade(controller(primary).Axis2.position(), controller(primary).Axis4.position() * TURN_FACTOR, STEER_BIAS);
      else
        chassis.control_arcade(controller(primary).Axis2.position(), controller(primary).Axis4.position(), STEER_BIAS);
    }
    wait(20, msec); // Sleep the task for a short amount of time
  }
}

void set_drive_mode() {
  controller(primary).rumble("-");
  drive_mode = (drive_mode + 1) % 2;
  if (drive_mode == 0) controller(primary).Screen.print("double arcade         ");
  if (drive_mode == 1) controller(primary).Screen.print("tank drive            ");
}

bool exit_auton_menu = false;
void autonomous(void) {
  exit_auton_menu = true;
  run_auton_item(current_auton_selection);
}

void print_menu_item(char const * txt[], char const * title) {
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1, 1);
  Brain.Screen.print("%s", title);
  Brain.Screen.setCursor(3, 1);
  Brain.Screen.print("%s", txt[current_auton_selection]);
  controller(primary).Screen.print("%s %s     ", txt[current_auton_selection], title);
}

void print_menu(char const * txt[], char const * title) {
  int auton_num = sizeof(txt) / sizeof(txt[0]);
  Brain.Screen.setFont(mono30);
  print_menu_item(txt, title);

  while (!exit_auton_menu) {
    if (Brain.Screen.pressing()) {
      while (Brain.Screen.pressing()) {
        //wait until finger lifted up from the screen
        wait(20, msec);
      }
      current_auton_selection = (current_auton_selection + 1) % auton_num;
      print_menu_item(txt, title);
      controller(primary).rumble(".");
    }
    task::sleep(50);
  }
  Brain.Screen.setFont(mono20);
}

void pre_auton() {
  setup_gyro();
  check_motors();
  reset_chassis();
  show_auton_menu();
}

void setup_gyro() {
  while (chassis.Gyro.isCalibrating()) {
    wait(25, msec);
  }
  controller(primary).rumble("-");
  if (!chassis.Gyro.installed()) {
    controller(primary).Screen.print("inertial sensor failure");
    controller(primary).rumble("----");
  }
}

const int MOTOR_COUNT = 6;
const int MOTOR_HEAT_LIMIT = 45;
bool check_motors() {
  int count = 0;
  int t = 0;
  for (int i = 0; i < 20; i++) {
    motor m = motor(i);
    if (m.installed()) {
      count++;
      t = m.temperature(celsius);
      if (t >= MOTOR_HEAT_LIMIT) {
        controller(primary).Screen.print("motor %d is %dC", i + 1, t);
        controller(primary).rumble("----");
        return false;
      }
    }
  }
  if (count < MOTOR_COUNT) {
    controller(primary).Screen.print("%d motor is disconnected      ", MOTOR_COUNT - count);
    controller(primary).rumble("----");
    return false;
  }
  return true;
}

void reset_chassis() {
  chassis.set_heading(inertial1.heading());
  chassis.LDrive.resetPosition();
  chassis.RDrive.resetPosition();
  chassis.LDrive.stop(coast);
  chassis.RDrive.stop(coast);

  chassis.set_drive_constants(10, 1.5, 0, 10, 0);
  chassis.set_heading_constants(6, .4, 1);
  chassis.set_turn_constants(10, 0.2, .015, 1.5, 7.5);

  // Each exit condition set is in the form (settle_error, settle_time, timeout).
  chassis.set_drive_exit_conditions(1, 300, 2000);
  chassis.set_turn_exit_conditions(1.5, 300, 1500);
}
