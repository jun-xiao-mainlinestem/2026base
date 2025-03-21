

bool catcher_is_down = true;
void toggle_catcher() {
  if (catcher_is_down) {
    catcher_up();
    catcher_is_down = false;
  } 
  else {
    if (!catcher_out) {
      catcher_down();
      catcher_is_down = true;
    } else {
        catcher_swing_back();
        catcher_out = false; 
    }
  }
}


void turn_catcher_angle(double angle, double kp =0.4, double ki = 0.02, double kd =2, double settle_error=2) {
  if (!catcherSensor.installed()) return;
  
//  PID clampPID(angle - clampSensor.angle(deg), 0.2, 0.015, 1.5, 7.5, 3, 200, 1000);
  PID catcherPID(angle - catcherSensor.position(deg), kp, ki, kd, 15, settle_error, 200, 1500);

  while (!catcherPID.is_settled()) {
    float error = angle - catcherSensor.position(deg);
    // printf("angle %f\n", clampSensor.angle());
    float output = catcherPID.compute(error);
    output = clamp(output, -2, 2);
    catcherMotor.spin(forward, output, volt);
    task::sleep(10);
  }
  catcherMotor.stop(hold);
}

const double catcher_angle = 15;
void catcher_swing_back()
{
    turn_catcher_angle(catcher_angle);
}

void catcher_up()
{
    turn_catcher_angle(catcher_angle);
    controller(primary).Screen.print("catcher on          ");
    controller(primary).rumble("-");
}

void catcher_down()
{
    catcherMotor.spinFor(reverse, catcherSensor.position(deg) *60/36, degrees, 40, velocityUnits::pct);
    catcherMotor.stop(coast);
    controller(primary).Screen.print("catcher down          ");
}

void menu_preset() {
  wait(0.5, seconds);
  int auton_choice = 0;
  if (Brain.ThreeWirePort.G.value() > 2000) {
    auton_choice = 1;
  }

  if (Brain.ThreeWirePort.F.value() > 2000) {
    auton_choice = 2;
  }

  float hue = license_optical.hue();
  if (hue > 200) {
    team_color = 1; // blue
  }

  current_auton_selection = auton_choice * 2 + team_color;
}


void red_4rings()
{
      chassis.set_heading(180);

    //dump the first ring
    chassis.set_heading(180);
    chassis.drive_distance(-24, 150, 5, 3);
    clamp_mobile_goal();
    intake.spin(forward, 12, volt);
    wait(300, msec);

    //get the second ring
    chassis.turn_to_heading(280, 5);
  chassis.drive_distance(22);
    wait(300, msec);
    
  // get the third ring
    chassis.turn_to_heading(350, 5);
    chassis.drive_distance(13);
    wait(300, msec);

    // get the forth ring
    chassis.drive_distance(-6, 5);
    chassis.turn_to_heading(330);
    chassis.drive_distance(11);
    wait(300, msec);

       //get to the ladder
    chassis.drive_distance(-27);
    intake.stop(coast);
    chassis.turn_to_heading(225);
    chassis.drive_distance(-30);

}

  release_mobile_goal();

  // get to the first mobile goal 
  chassis.set_heading(180);
  chassis.drive_distance(-24, 6);
  chassis.drive_distance(-23, 150, 5, 3);
  clamp_mobile_goal();
  intake.spin(forward, 12, volt);

  // intake the 2nd ring
  chassis.turn_to_heading(185, 6);
  chassis.drive_distance(16);
  wait(1000, msec);
  release_mobile_goal();
  intake.stop();

  // get the 2nd mogo
  chassis.turn_to_heading(90);
  chassis.drive_distance(-20);
  clamp_mobile_goal();
  chassis.turn_to_heading(230);

  // get the 3rd ring
  intake.spin(forward, 12, volt);
  enable_color_sorting();
  chassis.drive_distance(24);
  chassis.drive_distance(22, 4);
  wait(1000, msec);
  disable_color_sorting();
  release_mobile_goal();
  intake.stop();



void red_solo() {
  release_mobile_goal();
  chassis.set_heading(180);

  //get the first mobile goal
  chassis.drive_distance(-24, 150, 5, 3);
  clamp_mobile_goal();
  intake.spin(forward, 12, volt);
  wait(300, msec);
  chassis.turn_to_heading(236, 5);
  release_mobile_goal();

  //get the second ring, dump the other ring on the way
  chassis.drive_distance(16);
  chassis.drive_distance(27, 3.5);
  intake.stop(coast);

  //dump the ring to the second mobile goal
  chassis.turn_to_heading(150);
  chassis.drive_distance(-24);
  chassis.drive_distance(-8.5, 4);
  intake.spin(forward, 12, volt);
  clamp_mobile_goal();
  wait(300, msec);

  //get the third ring
  chassis.turn_to_heading(270, 5);
  chassis.drive_distance(19.5);
  wait(300, msec);

  //todo: get to the ladder
}

//get the second ring
chassis.turn_to_heading(280, 5);
chassis.drive_distance(22); wait(300, msec);

// get the third ring chassis.turn_to_heading(350, 5); chassis.drive_distance(13); wait(300, msec);

// get the forth ring
chassis.drive_distance(-6, 5);
chassis.turn_to_heading(330);
chassis.drive_distance(11);
wait(300, msec);



  //get the second mobile goal
  chassis.turn_to_heading(team_color * 90);
  chassis.drive_distance(-24, 6);
  clamp_mobile_goal();

  chassis.turn_to_heading(team_color * 135);
  intake.spin(forward, 12, volt);
  enable_color_sorting();
  chassis.drive_distance(42); 

  chassis.drive_with_voltage(4, 4);
  wait(800, msec);
  chassis.drive_with_voltage(-5, -5);
  wait(500, msec);
  chassis.drive_with_voltage(4, 4);
  wait(800, msec);
  chassis.drive_with_voltage(-5, -5);
  wait(500, msec);
  chassis.drive_with_voltage(4, 4);
  wait(800, msec);
  chassis.drive_with_voltage(-5, -5);
  wait(500, msec);
  chassis.drive_with_voltage(4, 4);
  wait(800, msec);
  chassis.drive_with_voltage(-5, -5);
  wait(500, msec);
  chassis.drive_with_voltage(0, 0);


bool intake_check_ok() {
  // if clamp somehow get loose, re-clamp
  if (clampMotor.torque() < 0.2) {
    clamp_mobile_goal();
  }

  // saftey measure: when hook gets stuck
  double c = intake.current();
  if (c > MAX_INTAKE_CURRENT) {
    intake.stop(coast);
    controller(primary).rumble("-");
    controller(primary).Screen.print("intake jam");
    return false;
  }
  return true;
}


    wait(500, msec);
 // release_mobile_goal();
  chassis.turn_to_heading(150);
  chassis.drive_distance(30);
  slapper_down();
  chassis.drive_with_voltage(-10, 10);
  wait(500, msec);
  chassis.drive_with_voltage(0, 0);
  slapper_up();
  chassis.turn_to_heading(120);



  void elimination()
{
  release_mobile_goal();

  // get to the first mobile goal 
  chassis.set_heading(180);
  chassis.drive_distance(-24, 6);
  chassis.drive_distance(-22, team_color * 150, 5, 3);
  clamp_mobile_goal();
  disable_auto_clamp();
  wait(200, msec);
  intake.spin(forward, 12, volt);
  chassis.turn_to_heading(team_color * 190, 3);


  // get the second ring
  clampMotor.setVelocity(100, pct);
  clampMotor.spinFor(reverse, 100, degrees, false);
  wait(100, msec);
  chassis.drive_distance(17);
  intake.stop(coast);

  //get the second mobile goal
  chassis.turn_to_heading(team_color * 90);
  chassis.drive_distance(-20, 5);
  clamp_mobile_goal();
  intake.spin(forward, 12, volt); 
  wait(600, msec);

  //clear rings
  intake.spin(reverse, 10, volt);
  chassis.drive_distance(24, 5);
  chassis.turn_to_heading(190);
  chassis.drive_distance(28);
  chassis.drive_distance(-3);
  chassis.turn_to_heading(135);
  intake.stop();
}
