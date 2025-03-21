#pragma once
#include "vex.h"

class Drive
{
private:
  float wheel_diameter;
  float wheel_ratio;
  float gyro_scale = 360;
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
  float heading_ki;
  float heading_kd;
  float heading_starti;

  float k_brake = 0.16,  k_throttle = 5,  k_turn = 10;

  bool stop_hold = false;

  float get_left_position_in();
  float get_right_position_in();

  void set_drive_constants(float drive_max_voltage, float drive_kp, float drive_ki, float drive_kd, float drive_starti);
  void set_heading_constants(float heading_max_voltage, float heading_kp, float heading_ki, float heading_kd, float heading_starti);
  void set_turn_exit_conditions(float turn_settle_error, float turn_settle_time, float turn_timeout);
  void set_turn_constants(float turn_max_voltage, float turn_kp, float turn_ki, float turn_kd, float turn_starti); 


public: 
  motor_group LDrive;
  motor_group RDrive;
  inertial Gyro;
  float desired_distance;
  float desired_heading;

  Drive(motor_group LDrive, motor_group RDrive, inertial gyro, float wheel_diameter, float wheel_ratio);

  float get_absolute_heading();
  void set_heading(float orientation_deg);

  void drive_with_voltage(float leftVoltage, float rightVoltage);

  void turn_to_heading(float heading);
  void turn_to_heading(float heading, float turn_max_voltage);
  void turn_to_heading(float heading, float turn_max_voltage, bool nonestop);


  void drive_distance(float distance);
  void drive_distance(float distance, float drive_max_voltage);
  void drive_distance(float distance, float drive_max_voltage, float heading);
  void drive_distance(float distance, float drive_max_voltage, float heading, float heading_max_voltage);

  bool DrivetrainNeedsToBeStopped = false;
  void reset(int max_drive_voltage = 10, int max_turn_voltage = 10, float turn_kp = 0.2, float turn_settle_error = 1.5);

  void control_arcade(int throttle, int turn, float desaturateBias = 0);
  void control_tank(int left, int right);

  double k_cutoff_angle = 15;
  double k_cutoff_time = 20;

  void set_drive_exit_conditions(float drive_settle_error, float drive_settle_time, float drive_timeout);
  void set_stopping(bool set_hold = true);
  void drive_distance(float distance, float drive_max_voltage, float heading, float heading_max_voltage, float turn_settle_error, float turn_settle_time);

};