#pragma once
#include "vex.h"

// A class to control the robot's drivetrain.
class Drive
{
private:
  // The diameter of the wheels.
  float wheel_diameter;
  // The gear ratio of the drivetrain.
  float gear_ratio;
  // The ratio to convert from inches to degrees of wheel rotation.
  float drive_in_to_deg_ratio;

  // PID constants for turning.
  float turn_max_voltage;
  float turn_kp;
  float turn_ki;
  float turn_kd;
  float turn_starti;
  float turn_settle_error;
  float turn_settle_time;
  float turn_timeout;

  // PID constants for driving.
  float drive_max_voltage;
  float drive_kp;
  float drive_ki;
  float drive_kd;
  float drive_starti;
  float drive_settle_error;
  float drive_settle_time;
  float drive_timeout;

  // PID constants for maintaining heading while driving.
  float heading_max_voltage;
  float heading_kp;
  float heading_kd;

  // Constants for driver control.
  float k_brake = 0.16;
  float k_throttle = 5;
  float k_turn = 10;

  // The default brake type for the drivetrain.
  vex::brakeType stop_mode = coast;
  
  // The motor group for the left side of the drivetrain.
  motor_group LDrive;
  // The motor group for the right side of the drivetrain.
  motor_group RDrive;
  // The inertial sensor.
  inertial Gyro;

public: 
  // The desired heading of the robot.
  float desired_heading;

  // The constructor for the Drive class.
  Drive(motor_group LDrive, motor_group RDrive, inertial gyro, float wheel_diameter, float gear_ratio);

  // Gets the current heading of the robot.
  float get_heading();
  // Sets the current heading of the robot.
  void set_heading(float orientation_deg);

  // Drives the robot with a specific voltage for each side of the drivetrain.
  void drive_with_voltage(float leftVoltage, float rightVoltage);

  // Turns the robot to a specific heading.
  void turn_to_heading(float heading);
  // Turns the robot to a specific heading with a maximum voltage.
  void turn_to_heading(float heading, float turn_max_voltage);
  // Turns the robot to a specific heading with advanced options.
  void turn_to_heading(float heading, float turn_max_voltage, bool nonstop, float settle_error = 5, float settle_time = 50);

  // Drives the robot a specific distance.
  void drive_distance(float distance);
  // Drives the robot a specific distance with a maximum voltage.
  void drive_distance(float distance, float drive_max_voltage);
  // Drives the robot a specific distance while maintaining a heading.
  void drive_distance(float distance, float drive_max_voltage, float heading, float heading_max_voltage);
  // Drives the robot a specific distance with advanced options.
  void drive_distance(float distance, float drive_max_voltage, float heading, float heading_max_voltage, bool nonstop, float drive_settle_error=2, float drive_settle_time=50) ;

  // A flag to indicate if the drivetrain needs to be stopped.
  bool drivetrain_needs_stopped = false;
  // A flag to disable driver control.
  bool driver_control_disabled = false;

  // Controls the robot in arcade mode.
  void control_arcade(int throttle, int turn, float turnBias = 0);
  // Controls the robot in tank mode.
  void control_tank(int left, int right);

  // Sets the PID constants for driving.
  void set_drive_constants(float drive_max_voltage, float drive_kp, float drive_ki, float drive_kd, float drive_starti);
  // Sets the exit conditions for driving.
  void set_drive_exit_conditions(float drive_settle_error, float drive_settle_time, float drive_timeout);
  // Sets the PID constants for maintaining heading.
  void set_heading_constants(float heading_max_voltage, float heading_kp, float heading_kd);
  // Sets the exit conditions for turning.
  void set_turn_exit_conditions(float turn_settle_error, float turn_settle_time, float turn_timeout);
  // Sets the PID constants for turning.
  void set_turn_constants(float turn_max_voltage, float turn_kp, float turn_ki, float turn_kd, float turn_starti); 
  // Stops the drivetrain.
  void stop(vex::brakeType mode);

  // Gets the position of the left side of the drivetrain in inches.
  float get_left_position_in();
  // Gets the position of the right side of the drivetrain in inches.
  float get_right_position_in();
};