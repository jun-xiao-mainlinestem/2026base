#include "vex.h"
using namespace vex;

motor intake = motor(PORT8, ratio18_1, true);
motor clampMotor = motor(PORT7, ratio36_1, false);
rotation clampRotation = rotation(PORT10, true);
distance mogo_distance = distance(PORT11);

void detect_mogo(){
  if (mogo_distance.objectDistance(inches) < 1 && clamp_is_up){
    clamp_mogo();
  }
}

void pre_auton() {
  setup_gyro();
  check_motors(6, 45);
  reset_chassis();
  show_auton_menu();
  // additional setup for other systems of the robot
  clampRotation.setPosition(clampRotation.angle(deg), degrees);
  mogo_distance.changed(detect_mogo);
}

void rotate_clamp_to(float angle)
{
  PID clampPID(angle- clampRotation.position(deg), 0.4, 0.2, 0, 15, 3, 200, 1000);
  while (clampPID.is_done() == false) {
    float error = (angle - clampRotation.position(deg));
    float output = clampPID.compute(error);
    output = threshold(output, -10, 10);
    clampMotor.spin(forward, output, volt);
    task::sleep(10);
  }
  clampMotor.stop(hold);
}

void clamp_mogo(){
  rotate_clamp_to(240);
}

void release_mogo()
{
  rotate_clamp_to(40);
}

bool clamp_is_up = true;