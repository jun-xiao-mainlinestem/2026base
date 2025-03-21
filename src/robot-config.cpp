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

// constant definitions
// for driving control
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


int drive_mode = 1;
// 0: single hand arcade  1: both hand arcade   2: tank
bool macro_running = false;

void usercontrol(void) {

 // task end_game_reminder(endgame_timer);
 return;

  while (1) {
    if (macro_running) {
      wait(20, msec);
      continue;
    }

    if (drive_mode == 0) chassis.control_arcade(controller(primary).Axis3.position(), controller(primary).Axis4.position() * TURN_FACTOR * TURN_FACTOR, STEER_BIAS);
    if (drive_mode == 2) chassis.control_tank(controller(primary).Axis3.position(), controller(primary).Axis2.position());
    if (drive_mode == 1) 
    {
      if (abs(controller(primary).Axis4.position()) < 100)
        chassis.control_arcade(controller(primary).Axis2.position(), controller(primary).Axis4.position() * TURN_FACTOR, STEER_BIAS);
      else 
        chassis.control_arcade(controller(primary).Axis2.position(), controller(primary).Axis4.position(), STEER_BIAS);
    } 

    wait(20, msec); // Sleep the task for a short amount of time
  }
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
// for motor check
const int MOTOR_COUNT = 8;
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
        controller(primary).Screen.print("motor %d is %dC", i+1, t);
        controller(primary).rumble("----");
        return false;
      }
    }
  }
  controller(primary).Screen.print("                ");

  if (count < MOTOR_COUNT) {
    controller(primary).Screen.print("%d motor is disconnected", MOTOR_COUNT - count);
    controller(primary).rumble("----");
    return false;
  }

  return true;
}


void show_temperature()
{
    Brain.Screen.clearScreen();
    int row = 1;

    for (int i=0; i < 20; i++)
    {
        motor m = motor(i);
        if(m.installed())
        {
            Brain.Screen.setCursor(row, 1);
          //  float t = m.temperature(percentUnits::pct);
          //  Brain.Screen.print("port%d: %2.0f%%", i+1, t);

            int t = m.temperature(celsius);
            Brain.Screen.print("port%d: %dC", i+1, t);

            row ++;
        }
    }

}

int show_heading() {
  chassis.set_heading(0);
  while (true) {
  wait(300, msec);
  float h = chassis.get_absolute_heading();

  controller(primary).Screen.print("heading: %4.1f, d: %3.1f", h, d);
  Brain.Screen.setCursor(11, 1);
  Brain.Screen.print("heading: %4.1f", h);
  Brain.Screen.setCursor(12, 1);
  Brain.Screen.print("wall distance: %4.1f", d);
  }
  return 1;
}

void pre_auton() {
  setup_gyro();
  check_motors();
  show_temperature();
  chassis.reset();
  release_mobile_goal();
  task show_heading_task(show_heading);
}