#include "vex.h"

Drive::Drive(motor_group LDrive, motor_group RDrive, inertial gyro, float wheel_diameter, float gear_ratio):
  wheel_diameter(wheel_diameter),
  gear_ratio(gear_ratio),
  drive_in_to_deg_ratio(gear_ratio / 360.0 * M_PI * wheel_diameter),
  LDrive(LDrive),
  RDrive(RDrive),
  Gyro(gyro) {}

void Drive::set_turn_constants(float turn_max_voltage, float turn_kp, float turn_ki, float turn_kd, float turn_starti) {
  this -> turn_max_voltage = turn_max_voltage;
  this -> turn_kp = turn_kp;
  this -> turn_ki = turn_ki;
  this -> turn_kd = turn_kd;
  this -> turn_starti = turn_starti;
}

void Drive::set_drive_constants(float drive_max_voltage, float drive_kp, float drive_ki, float drive_kd, float drive_starti) {
  this -> drive_max_voltage = drive_max_voltage;
  this -> drive_kp = drive_kp;
  this -> drive_ki = drive_ki;
  this -> drive_kd = drive_kd;
  this -> drive_starti = drive_starti;
}

void Drive::set_heading_constants(float heading_max_voltage, float heading_kp, float heading_kd) {
  this -> heading_max_voltage = heading_max_voltage;
  this -> heading_kp = heading_kp;
  this -> heading_kd = heading_kd;
}

void Drive::set_turn_exit_conditions(float turn_settle_error, float turn_settle_time, float turn_timeout) {
  this -> turn_settle_error = turn_settle_error;
  this -> turn_settle_time = turn_settle_time;
  this -> turn_timeout = turn_timeout;
}

void Drive::set_drive_exit_conditions(float drive_settle_error, float drive_settle_time, float drive_timeout) {
  this -> drive_settle_error = drive_settle_error;
  this -> drive_settle_time = drive_settle_time;
  this -> drive_timeout = drive_timeout;
}

void Drive::set_heading(float orientation_deg) {
  Gyro.setHeading(orientation_deg, deg);
  desired_heading = orientation_deg;
}

float Drive::get_heading() {
  return (Gyro.heading());
}

float Drive::get_left_position_in() {
  return (LDrive.position(deg) * drive_in_to_deg_ratio);
}

float Drive::get_right_position_in() {
  return (RDrive.position(deg) * drive_in_to_deg_ratio);
}

void Drive::drive_with_voltage(float leftVoltage, float rightVoltage) {

  LDrive.spin(fwd, leftVoltage, volt);
  RDrive.spin(fwd, rightVoltage, volt);
}

void Drive::turn_to_heading(float heading) {
  turn_to_heading(heading, turn_max_voltage);
}

void Drive::turn_to_heading(float heading, float turn_max_voltage) {
  turn_to_heading(heading, turn_max_voltage, false, turn_settle_error, turn_settle_time);
}

/*
 * Turns the robot to a specified angle using PID control.
 * - heading: Target heading to turn to (in degrees).
 * - turn_max_voltage: Maximum voltage for the turn 
 * - nonstop: If false, stops the robot at the end; if true, allows chaining.
 * - settle_error: error range for start calcuating the settle time.
 * - settle_time: settle time for existing the PID loop (in milliseconds).
 */

void Drive::turn_to_heading(float heading, float turn_max_voltage, bool nonstop, float settle_error, float settle_time) {
  desired_heading = reduce_0_to_360(heading);
  PID turnPID(reduce_negative_180_to_180(heading - get_heading()), turn_kp, turn_ki, turn_kd, turn_starti, settle_error, settle_time, turn_timeout);
  while (!turnPID.is_done()) {
    float error = reduce_negative_180_to_180(heading - get_heading());
    float output = turnPID.compute(error);
    output = threshold(output, -turn_max_voltage, turn_max_voltage);
    drive_with_voltage(output, -output);
    wait(10, msec);
  }
  if (!nonstop) {
    LDrive.stop(hold);
    RDrive.stop(hold);
  }
}

void Drive::drive_distance(float distance) {
  drive_distance(distance, drive_max_voltage, desired_heading, heading_max_voltage, false, drive_settle_error, drive_settle_time);
}

void Drive::drive_distance(float distance, float drive_max_voltage) {
  drive_distance(distance, drive_max_voltage, desired_heading, heading_max_voltage, false, drive_settle_error, drive_settle_time);
}

void Drive::drive_distance(float distance, float drive_max_voltage, float heading, float heading_max_voltage) {
  drive_distance(distance, drive_max_voltage, heading, heading_max_voltage, false, drive_settle_error, drive_settle_time);

}

void Drive::drive_distance(float distance, float drive_max_voltage, float heading, float heading_max_voltage, bool nonstop, float drive_settle_error, float drive_settle_time) {
  desired_heading = reduce_0_to_360(heading);

  PID drivePID(distance, drive_kp, drive_ki, drive_kd, drive_starti, drive_settle_error, drive_settle_time, drive_timeout);
  PID headingPID(reduce_negative_180_to_180(desired_heading - get_heading()), heading_kp, heading_kd);
  float start_average_position = (get_left_position_in() + get_right_position_in()) / 2.0;
  float average_position = start_average_position;
  while (drivePID.is_done() == false && !drivetrain_needs_stopped) {
    average_position = (get_left_position_in() + get_right_position_in()) / 2.0;
    float drive_error = distance + start_average_position - average_position;
    float heading_error = reduce_negative_180_to_180(desired_heading - get_heading());
    float drive_output = drivePID.compute(drive_error);
    float heading_output = headingPID.compute(heading_error);

    drive_output = threshold(drive_output, -drive_max_voltage, drive_max_voltage);
    heading_output = threshold(heading_output, -heading_max_voltage, heading_max_voltage);

    drive_with_voltage(drive_output + heading_output, drive_output - heading_output);
    wait(10, msec);
  }
  drivetrain_needs_stopped = false;
  if (!nonstop) {
    LDrive.stop(hold);
    RDrive.stop(hold);
  }
}

double curve_function(double x, double curve_scale) {
  return (powf(2.718, -(curve_scale / 10)) + powf(2.718, (fabs(x) - 100) / 10) * (1 - powf(2.718, -(curve_scale / 10)))) * x;
}

void Drive::control_arcade(int y, int x, float turnBias) {
  if (driver_control_disabled) return;
  float throttle = deadband(y, 5);
  float turn = deadband(x, 5);

  turn = curve_function(turn, k_turn);
  throttle = curve_function(throttle, k_throttle);

  float left_power = to_volt(throttle + turn);
  float right_power = to_volt(throttle - turn);

  if (turnBias > 0) {
    if (fabs(throttle) + fabs(turn) > 100) {
      int oldThrottle = throttle;
      int oldTurn = turn;
      throttle *= (1 - turnBias * fabs(oldTurn / 100.0));
      turn *= (1 - (1 - turnBias) * fabs(oldThrottle / 100.0));
    }
    left_power = to_volt(throttle + turn);
    right_power = to_volt(throttle - turn);
  }

  if (fabs(throttle) > 0 || fabs(turn) > 0) {
    LDrive.spin(fwd, left_power, volt);
    RDrive.spin(fwd, right_power, volt);
    LDrive.resetPosition();
    RDrive.resetPosition();
    drivetrain_needs_stopped = true;
  }
  // When joystick are released, run active brake on drive
  // ajdust the coefficient to the amount of coasting preferred
  else {
    if (drivetrain_needs_stopped) {
      if (stop_mode != hold) {
        LDrive.spin(fwd, -LDrive.position(rev) * k_brake, volt);
        RDrive.spin(fwd, -RDrive.position(rev) * k_brake, volt);
      } else {
        LDrive.stop(hold);
        RDrive.stop(hold);
      }
      float h = get_heading();
      controller(primary).Screen.print("heading: %4.1f", h);
      drivetrain_needs_stopped = false;
    }
  }
}

void Drive::control_tank(int left, int right) {
  if (driver_control_disabled) return;
  float leftthrottle = curve_function(left, k_throttle);
  float rightthrottle = curve_function(right, k_throttle);

  if (fabs(leftthrottle) > 0 || fabs(rightthrottle) > 0) {
    LDrive.spin(fwd, to_volt(leftthrottle), volt);
    RDrive.spin(fwd, to_volt(rightthrottle), volt);
    drivetrain_needs_stopped = true;
  } else {
    if (drivetrain_needs_stopped) {
      LDrive.stop(stop_mode);
      RDrive.stop(stop_mode);
      drivetrain_needs_stopped = false;
    }
  }
}

void Drive::stop(vex::brakeType mode) {
    LDrive.stop(mode);
    RDrive.stop(mode);
    stop_mode = mode;
    chassis.LDrive.resetPosition();
    chassis.RDrive.resetPosition();
}