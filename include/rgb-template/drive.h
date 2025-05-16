#pragma once
#include "vex.h"

class Drive
{
private:
  float wheel_diameter;
  float gear_ratio;
  float drive_in_to_deg_ratio;

  float turn_max_voltage;
  float turn_kp;
  float turn_ki;
  float turn_kd;
  float turn_starti;
  float turn_settle_error;
  float turn_settle_time;
  float turn_timeout;

  float drive_max_voltage;
  float drive_kp;
  float drive_ki;
  float drive_kd;
  float drive_starti;
  float drive_settle_error;
  float drive_settle_time;
  float drive_timeout;


  float heading_max_voltage;
  float heading_kp;
  float heading_kd;

// used in driver control
  float k_brake = 0.16;
  float k_throttle = 5;
  float k_turn = 10;

  vex::brakeType stop_mode = coast;

  float get_left_position_in();
  float get_right_position_in();
  
  motor_group LDrive;
  motor_group RDrive;
  inertial Gyro;

public: 
  float desired_heading;

  Drive(motor_group LDrive, motor_group RDrive, inertial gyro, float wheel_diameter, float gear_ratio);

  float get_heading();
  void set_heading(float orientation_deg);

  void drive_with_voltage(float leftVoltage, float rightVoltage);

  void turn_to_heading(float heading);
  void turn_to_heading(float heading, float turn_max_voltage);
  void turn_to_heading(float heading, float turn_max_voltage, bool nonstop, float settle_error = 5, float settle_time = 50);

  void drive_distance(float distance);
  void drive_distance(float distance, float drive_max_voltage);
  void drive_distance(float distance, float drive_max_voltage, float heading, float heading_max_voltage);
  void drive_distance(float distance, float drive_max_voltage, float heading, float heading_max_voltage, bool nonstop, float drive_settle_error=2, float drive_settle_time=50) ;

  bool drivetrain_needs_stopped = false;
  bool driver_control_disabled = false;

  void control_arcade(int throttle, int turn, float turnBias = 0);
  void control_tank(int left, int right);

  void set_drive_constants(float drive_max_voltage, float drive_kp, float drive_ki, float drive_kd, float drive_starti);
  void set_drive_exit_conditions(float drive_settle_error, float drive_settle_time, float drive_timeout);
  void set_heading_constants(float heading_max_voltage, float heading_kp, float heading_kd);
  void set_turn_exit_conditions(float turn_settle_error, float turn_settle_time, float turn_timeout);
  void set_turn_constants(float turn_max_voltage, float turn_kp, float turn_ki, float turn_kd, float turn_starti); 
  void stop(vex::brakeType mode);

};