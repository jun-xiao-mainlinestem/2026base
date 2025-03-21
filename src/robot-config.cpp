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

motor clampMotor = motor(PORT7, ratio36_1, false);
motor intake = motor(PORT8, ratio18_1, true);
rotation clampSensor = rotation(PORT10, true);


// constant definitions
// for driving control
const float TURN_FACTOR = 0.85;
const float STEER_BIAS = 0.5;


//smaller degree means clamp more
int CLAMP_ANGLE = 170;
double catcher_angle = 35;

int RELEASE_ANGLE = CLAMP_ANGLE + 105;
int RELEASE_ANGLE_HIGH = RELEASE_ANGLE + 100;
int STOP_ANGLE_BOTTOM = CLAMP_ANGLE -100;
int STOP_ANGLE_TOP = 600; 

distance front_distance = distance(PORT14);


Drive chassis(
  //Left Motors:
  motor_group(leftMotor1, leftMotor2, leftMotor3),

  //Right Motors:
  motor_group(rightMotor1, rightMotor2, rightMotor3),

  //Specify the PORT NUMBER of your inertial sensor, in PORT format (i.e. "PORT1", not simply "1"):
  PORT9,

  //****Input your wheel diameter. (4" omnis are actually closer to 4.125") ****

  2.75,

  //External ratio, must be in decimal, in the format of input teeth/output teeth.
  //If your motor has an 84-tooth gear and your wheel has a 60-tooth gear, this value will be 1.4.
  //If the motor drives the wheel directly, this value is 1:
  0.75

);

bool auto_clamp = false;
void set_auto_clamp(bool flag) {
  auto_clamp = flag;
}

bool clamp_is_down = false;
bool clamp_is_top = false;
bool clamp_is_running = false;

int ring_color = 1;

bool clamp_is_working = false;
void turn_clamp_angle(double angle, double kp =0.4, double ki = 0.02, double kd =2, double settle_error=3) {
  if (!clampSensor.installed()) return;
  if (clamp_is_working) return;
  clamp_is_working = true;
  
//  PID clampPID(angle - clampSensor.angle(deg), 0.2, 0.015, 1.5, 7.5, 3, 200, 1000);
  PID clampPID(angle - clampSensor.position(deg), kp, ki, kd, 15, settle_error, 200, 1500);
  while (!clampPID.is_settled() && clampMotor.torque()< 1.9) {
    float error = angle - clampSensor.position(deg);
    // printf("angle %f\n", clampSensor.angle());
    float output = clampPID.compute(error);
    // printf("output %f\n",output);
    output = clamp(output, -12, 12);
    clampMotor.spin(reverse, output, volt);
    task::sleep(10);
  }
  clampMotor.stop(hold);
  clamp_is_working = false;
}

void clamp_mobile_goal() {
  if (clamp_is_running || clamp_is_down) return;
  clamp_is_running = true;
  clamp_is_down = true;
  turn_clamp_angle(CLAMP_ANGLE);
  controller(primary).rumble(".");
  set_auto_clamp(false);
  clamp_is_running = false;
}

void release_mobile_goal() {
  if (clamp_is_running) return;
  clamp_is_running = true;
  turn_clamp_angle(RELEASE_ANGLE);
  clampMotor.stop(coast);
  clamp_is_running = false;
  set_auto_clamp(true);

  clamp_is_down = false;
}

void show_clamp_degree() {
  int p = clampSensor.angle(deg);
  controller(primary).Screen.print("degree: %4d  ", p);
}

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
  float d = front_distance.objectDistance(inches);

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