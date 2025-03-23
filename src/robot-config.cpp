#include "vex.h"
using namespace vex;

void pre_auton() {
  setup_gyro();
  check_motors(6, 45);
  reset_chassis();
  show_auton_menu();
  // additional setup for other systems of the robot
}
