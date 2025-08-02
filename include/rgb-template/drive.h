#pragma once
#include "vex.h"

// A class to control the robot's drivetrain.
class Drive
{
private:
  // The diameter of the wheels.
  float wheelDiameter;
  // The gear ratio of the drivetrain.
  float gearRatio;
  // The ratio to convert from inches to degrees of wheel rotation.
  float driveInToDegRatio;

  // PID constants for turning.
  float turnMaxVoltage;
  float turnKp;
  float turnKi;
  float turnKd;
  float turnStarti;
  float turnSettleError;
  float turnSettleTime;
  float turnTimeout;

  // PID constants for driving.
  float driveMaxVoltage;
  float driveKp;
  float driveKi;
  float driveKd;
  float driveStarti;
  float driveSettleError;
  float driveSettleTime;
  float driveTimeout;

  // PID constants for maintaining heading while driving.
  float headingMaxVoltage;
  float headingKp;
  float headingKd;

  // Constants for driver control.
  float kBrake = 0.16;
  float kThrottle = 5;
  float kTurn = 10;

  // The default brake type for the drivetrain.
  vex::brakeType stopMode = coast;
  
  // The motor group for the left side of the drivetrain.
  motor_group LDrive;
  // The motor group for the right side of the drivetrain.
  motor_group RDrive;
  // The inertial sensor.
  inertial Gyro;

  // Turns the robot to a specific heading with advanced options.
  void turnToHeading(float heading, float turnMaxVoltage, bool chaining, float settleError = 5, float settleTime = 50);

  // Drives the robot a specific distance with advanced options.
  void driveDistance(float distance, float driveMaxVoltage, float heading, float headingMaxVoltage, bool chaining, float driveSettleError=2, float driveSettleTime=50) ;

public: 
  // The desired heading of the robot.
  float desiredHeading;

  // The constructor for the Drive class.
  Drive(motor_group LDrive, motor_group RDrive, inertial gyro, float wheelDiameter, float gearRatio);

  // Gets the current heading of the robot.
  float getHeading();
  // Sets the current heading of the robot.
  void setHeading(float orientationDeg);

  // Drives the robot with a specific voltage for each side of the drivetrain.
  void driveWithVoltage(float leftVoltage, float rightVoltage);

  // Turns the robot to a specific heading.
  void turnToHeading(float heading);
  // Turns the robot to a specific heading with a maximum voltage.
  void turnToHeading(float heading, float turnMaxVoltage);

  // Drives the robot a specific distance.
  void driveDistance(float distance);
  // Drives the robot a specific distance with a maximum voltage.
  void driveDistance(float distance, float driveMaxVoltage);
  // Drives the robot a specific distance while maintaining a heading.
  void driveDistance(float distance, float driveMaxVoltage, float heading, float headingMaxVoltage);

  // A flag to indicate if the drivetrain needs to be stopped.
  bool drivetrainNeedsStopped = false;
  // A flag to disable driver control.
  bool driverControlDisabled = false;

  // Controls the robot in arcade mode.
  void controlArcade(int throttle, int turn, float turnBias = 0);
  // Controls the robot in tank mode.
  void controlTank(int left, int right);
  void controlMecanum(int x, int y, int acc, int steer, motor DriveLF, motor DriveLR, motor DriveRF, motor DriveRB);

  // Sets the PID constants for driving.
  void setDriveConstants(float driveMaxVoltage, float driveKp, float driveKi, float driveKd, float driveStarti);
  // Sets the exit conditions for driving.
  void setDriveExitConditions(float driveSettleError, float driveSettleTime, float driveTimeout);
  // Sets the PID constants for maintaining heading.
  void setHeadingConstants(float headingMaxVoltage, float headingKp, float headingKd);
  // Sets the exit conditions for turning.
  void setTurnExitConditions(float turnSettleError, float turnSettleTime, float turnTimeout);
  // Sets the PID constants for turning.
  void setTurnConstants(float turnMaxVoltage, float turnKp, float turnKi, float turnKd, float turnStarti); 
  // Stops the drivetrain.
  void stop(vex::brakeType mode);

  // Gets the position of the left side of the drivetrain in inches.
  float getLeftPositionIn();
  // Gets the position of the right side of the drivetrain in inches.
  float getRightPositionIn();
};