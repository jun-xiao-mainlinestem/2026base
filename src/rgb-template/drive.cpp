#include "vex.h"

Drive::Drive(motor_group LDrive, motor_group RDrive, inertial gyro, float wheel_diameter, float wheel_ratio):
  wheel_diameter(wheel_diameter),
  wheel_ratio(wheel_ratio),
  drive_in_to_deg_ratio(wheel_ratio / 360.0 * M_PI * wheel_diameter),
  LDrive(LDrive),
  RDrive(RDrive),
  //Every device below is passed by port rather than passing the object.
  //This ensures that the devices work properly.
  Gyro(gyro)
  {}

// All PID constants are passed as kP, kI, kD, and startI. The kP, kI, and kD are pretty standard,
// but startI keeps the integral value at 0 until the absolute value of the error is below startI.
// This prevents integral windup on bigger turns.

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

void Drive::set_heading_constants(float heading_max_voltage, float heading_kp, float heading_ki, float heading_kd, float heading_starti) {
  this -> heading_max_voltage = heading_max_voltage;
  this -> heading_kp = heading_kp;
  this -> heading_ki = heading_ki;
  this -> heading_kd = heading_kd;
  this -> heading_starti = heading_starti;
}

// Settle error and settle time work together to check whether the desired position was achieved, but
// timeout is separate. The robot must stay within the settle error for the duration of the settle time 
// to be settled. If the duration of the movement reaches timeout without being settled, the robot
// gives up and goes to the next movement. 

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
  Gyro.setRotation(orientation_deg * gyro_scale / 360.0, deg);
  desired_heading = orientation_deg;
}

float Drive::get_absolute_heading() {
  return (reduce_0_to_360(Gyro.rotation() * 360.0 / gyro_scale));
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
  desired_heading = reduce_0_to_360(heading);
  // Desired heading carries over the angle from one movement to another. That way, if the robot doesn't
  // finish a turn movement, it will still drive at the angle that was specified in the turn movement.
  PID turnPID(reduce_negative_180_to_180(heading - get_absolute_heading()), turn_kp, turn_ki, turn_kd, turn_starti, turn_settle_error, turn_settle_time, turn_timeout);
  while (turnPID.is_done() == false) {
    float error = reduce_negative_180_to_180(heading - get_absolute_heading());
    // Reducing the angle to a value between -180 and 180 degrees ensures that the robot always takes the 
    // shorter path when making a turn.
    float output = turnPID.compute(error);
    output = clamp(output, -turn_max_voltage, turn_max_voltage);
    drive_with_voltage(output, -output);
    task::sleep(10);
  }
    LDrive.stop(hold);
    RDrive.stop(hold);
}

void Drive::turn_to_heading(float heading, float turn_max_voltage, bool nonestop) {
  desired_heading = reduce_0_to_360(heading);
  // Desired heading carries over the angle from one movement to another. That way, if the robot doesn't
  // finish a turn movement, it will still drive at the angle that was specified in the turn movement.
  double se = turn_settle_error;
  double st = turn_settle_time;
  if (nonestop)  
  {
    se = k_cutoff_angle;
    st = k_cutoff_time;
  }
  PID turnPID(reduce_negative_180_to_180(heading - get_absolute_heading()), turn_kp, turn_ki, turn_kd, turn_starti, se, st, turn_timeout);
  while (turnPID.is_done() == false) {
    float error = reduce_negative_180_to_180(heading - get_absolute_heading());
    // Reducing the angle to a value between -180 and 180 degrees ensures that the robot always takes the 
    // shorter path when making a turn.
    float output = turnPID.compute(error);
    output = clamp(output, -turn_max_voltage, turn_max_voltage);
    drive_with_voltage(output, -output);
    task::sleep(10);
  }
  if (!nonestop)
  {
    LDrive.stop(hold);
    RDrive.stop(hold);
  }
}

void Drive::drive_distance(float distance) {
  drive_distance(distance, drive_max_voltage, desired_heading, heading_max_voltage);
}

void Drive::drive_distance(float distance, float drive_max_voltage) {
  drive_distance(distance, drive_max_voltage, desired_heading, heading_max_voltage);
}

void Drive::drive_distance(float distance, float drive_max_voltage, float heading){
  drive_distance(distance, drive_max_voltage, heading, heading_max_voltage);
}

void Drive::drive_distance(float distance, float drive_max_voltage, float heading, float heading_max_voltage) {
  desired_heading = reduce_0_to_360(heading);
  desired_distance = distance;

  PID drivePID(desired_distance, drive_kp, drive_ki, drive_kd, drive_starti, drive_settle_error, drive_settle_time, drive_timeout);
  PID headingPID(reduce_negative_180_to_180(heading - get_absolute_heading()), heading_kp, heading_ki, heading_kd, heading_starti);
  float start_average_position = (get_left_position_in() + get_right_position_in()) / 2.0;
  // Rather than resetting the drive position , this function just notes what the drive position started at
  // and determines error relative to that value.
  float average_position = start_average_position;
  while (drivePID.is_done() == false && !DrivetrainNeedsToBeStopped) {
    average_position = (get_left_position_in() + get_right_position_in()) / 2.0;
    float drive_error = desired_distance + start_average_position - average_position;
    float heading_error = reduce_negative_180_to_180(heading - get_absolute_heading());
    // Just like for turns, reducing from -180 to 180 degrees ensures that the robot takes the 
    // quickest path to the desired heading.
    float drive_output = drivePID.compute(drive_error);
    float heading_output = headingPID.compute(heading_error);

    drive_output = clamp(drive_output, -drive_max_voltage, drive_max_voltage);
    heading_output = clamp(heading_output, -heading_max_voltage, heading_max_voltage);

    drive_with_voltage(drive_output + heading_output, drive_output - heading_output);
    task::sleep(10);
  }
  DrivetrainNeedsToBeStopped = false;
    LDrive.stop(hold);
    RDrive.stop(hold);
}

void Drive::drive_distance(float distance, float drive_max_voltage, float heading, float heading_max_voltage, float drive_settle_error, float drive_settle_time) {
  desired_heading = reduce_0_to_360(heading);
  desired_distance = distance;

  PID drivePID(desired_distance, drive_kp, drive_ki, drive_kd, drive_starti, drive_settle_error, drive_settle_time, drive_timeout);
  PID headingPID(reduce_negative_180_to_180(heading - get_absolute_heading()), heading_kp, heading_ki, heading_kd, heading_starti);
  float start_average_position = (get_left_position_in() + get_right_position_in()) / 2.0;
  // Rather than resetting the drive position , this function just notes what the drive position started at
  // and determines error relative to that value.
  float average_position = start_average_position;
  while (drivePID.is_done() == false && !DrivetrainNeedsToBeStopped) {
    average_position = (get_left_position_in() + get_right_position_in()) / 2.0;
    float drive_error = desired_distance + start_average_position - average_position;
    float heading_error = reduce_negative_180_to_180(heading - get_absolute_heading());
    // Just like for turns, reducing from -180 to 180 degrees ensures that the robot takes the 
    // quickest path to the desired heading.
    float drive_output = drivePID.compute(drive_error);
    float heading_output = headingPID.compute(heading_error);

    drive_output = clamp(drive_output, -drive_max_voltage, drive_max_voltage);
    heading_output = clamp(heading_output, -heading_max_voltage, heading_max_voltage);

    drive_with_voltage(drive_output + heading_output, drive_output - heading_output);
    task::sleep(10);
  }
  DrivetrainNeedsToBeStopped = false;
}

double curve_function(double x, double curve_scale) {
  return (powf(2.718, -(curve_scale / 10)) + powf(2.718, (fabs(x) - 100) / 10) * (1 - powf(2.718, -(curve_scale / 10)))) * x;
}

void Drive::control_arcade(int y, int x, float desaturateBias) {
  float throttle = deadband(y, 5);
  float turn = deadband(x, 5);

  // adjust the parameter to RDrive's preference
  turn = curve_function(turn, k_turn);
  throttle = curve_function(throttle, k_throttle);

  float left_power = to_volt(throttle + turn);
  float  right_power = to_volt(throttle - turn);

  if (desaturateBias > 0)
  {
    if (fabs(throttle) + fabs(turn) > 100) {
        int oldThrottle = throttle;
        int oldTurn = turn;
        throttle *= (1 - desaturateBias * fabs(oldTurn / 100.0));
        turn *= (1 - (1 - desaturateBias) * fabs(oldThrottle / 100.0));
    }
    left_power = to_volt(throttle + turn);
    right_power = to_volt(throttle - turn);
  }

  if (fabs(throttle) > 0 || fabs(turn) > 0) {
    LDrive.spin(fwd, left_power, volt);
    RDrive.spin(fwd, right_power, volt);
    LDrive.resetPosition();
    RDrive.resetPosition();
    DrivetrainNeedsToBeStopped = true;
  }  
  // When joystick are released, run active brake on drive
  // ajdust the coefficient to the amount of coasting preferred
  else {
    if (DrivetrainNeedsToBeStopped) {
      if (!stop_hold)
      {
        LDrive.spin(fwd, -LDrive.position(rev) * k_brake, volt);
        RDrive.spin(fwd, -RDrive.position(rev) * k_brake, volt);
      }
      else
      {
        LDrive.stop(hold);
        RDrive.stop(hold);
      }
      DrivetrainNeedsToBeStopped = false;
    }
  }
}

void Drive::control_tank(int left, int right) {
  float leftthrottle = curve_function(left, k_throttle);
  float rightthrottle = curve_function(right, k_throttle);

  if (fabs(leftthrottle) > 0 || fabs(rightthrottle) > 0) {
    LDrive.spin(fwd, to_volt(leftthrottle), volt);
    RDrive.spin(fwd, to_volt(rightthrottle), volt);
    DrivetrainNeedsToBeStopped = true;
  } else {
    if (DrivetrainNeedsToBeStopped) {
      if (!stop_hold)
      {
        LDrive.stop(coast);
        RDrive.stop(coast);
      }
      else
      {
        LDrive.stop(hold);
        RDrive.stop(hold);
      }
      DrivetrainNeedsToBeStopped = false;
    }
  }
}

void Drive::reset(int max_drive_voltage, int max_turn_voltage, float turn_kp, float turn_settle_error)
{
  desired_heading = get_absolute_heading();
  LDrive.resetPosition();
  RDrive.resetPosition();
  LDrive.stop(coast);
  RDrive.stop(coast);

  // Each constant set is in the form of (maxVoltage, kP, kI, kD, startI).
  set_drive_constants(max_drive_voltage, 1.5, 0, 10, 0);
  set_heading_constants(6, .4, 0, 1, 0);

// 12, .3, .001, 2, 15 
  set_turn_constants(max_turn_voltage, turn_kp, .015, 1.5, 7.5);

  // Each exit condition set is in the form (settle_error, settle_time, timeout).
  set_drive_exit_conditions(1, 300, 2000);
  set_turn_exit_conditions(turn_settle_error, 300, 1500);
}



void Drive::set_stopping(bool set_hold)
{
  if (set_hold)
  {
    LDrive.stop(hold);
    RDrive.stop(hold);
    stop_hold = true;
  }
  else
  {
    LDrive.stop(coast);
    RDrive.stop(coast);
    stop_hold = false;    
  }

}

