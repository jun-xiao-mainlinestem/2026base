# `rgb-template` API Documentation

This document provides instructions on how to use the `rgb-template` library to program your VEX V5 robot.

## `Drive` Class

The `Drive` class is the core of the `rgb-template` library. It provides a high-level interface for controlling your robot's drivetrain.

### Initialization

To use the `Drive` class, you must first create an instance of it. The constructor takes the following arguments:

*   `LDrive`: A `motor_group` for the left side of the drivetrain.
*   `RDrive`: A `motor_group` for the right side of the drivetrain.
*   `gyro`: An `inertial` sensor for tracking the robot's orientation.
*   `wheel_diameter`: The diameter of the wheels in inches.
*   `gear_ratio`: The gear ratio of the drivetrain.

**Example from `src/chassis.cpp`:**

```cpp
#include "vex.h"

Drive chassis(
  //Left Motors:
  motor_group(leftMotor1, leftMotor2, leftMotor3),
  //Right Motors:
  motor_group(rightMotor1, rightMotor2, rightMotor3),
  inertial1,
  //wheel diameter:
  2.75,
  //Gear ratio of motor to wheel:
  0.75
);
```

### Autonomous Control

The `Drive` class provides several methods for controlling the robot during the autonomous period.

#### `turnToHeading`

This function turns the robot to a specific heading in degrees.

*   `turnToHeading(float heading)`: Turns the robot to a specific heading with default parameters.
*   `turnToHeading(float heading, float turnMaxVoltage)`: Turns the robot to a specific heading with a specified maximum voltage.
*   `turnToHeading(float heading, float turnMaxVoltage, bool nonstop, ...)`: Turns the robot to a specific heading with more control over the movement.

**Examples from `src/autons.cpp`:**

```cpp
// Simple turn
chassis.turnToHeading(90);

// Turn with specified max voltage and nonstop flag
chassis.turnToHeading(180, 10, true);
```

#### `driveDistance`

This function drives the robot a specific distance in inches.

*   `driveDistance(float distance)`: Drives a specific distance with default parameters.
*   `driveDistance(float distance, float driveMaxVoltage)`: Drives a specific distance with a specified maximum voltage.
*   `driveDistance(float distance, float driveMaxVoltage, float heading, float headingMaxVoltage)`: Drives a specific distance while maintaining a specific heading.
*   `driveDistance(float distance, float driveMaxVoltage, float heading, float headingMaxVoltage, bool nonstop, ...)`: Drives a specific distance with more control over the movement.

**Examples from `src/autons.cpp`:**

```cpp
// Drive a specific distance with a timeout
chassis.driveDistance(24);

// Drive a specific distance with a timeout and heading
chassis.driveDistance(24, 10, 0, 6, true);
```

### Driver Control

The `Drive` class also provides methods for controlling the robot during the driver control period.

*   `controlArcade(int throttle, int turn, float turnBias)`: Controls the robot using arcade-style controls.
*   `controlTank(int left, int right)`: Controls the robot using tank-style controls.

**Example from `src/chassis.cpp`:**

```cpp
void usercontrol(void) {
  // ...
  while (1) {
    if (DRIVE_TANK_MODE) chassis.controlTank(controller(primary).Axis3.position(), controller(primary).Axis2.position());
    else {
        chassis.controlArcade(controller(primary).Axis2.position(), controller(primary).Axis4.position());
    }
    wait(20, msec); 
  }
}
```

### Configuration

The `Drive` class provides several methods for configuring the drivetrain's PID constants and exit conditions.

*   `setDriveConstants(...)`
*   `setDriveExitConditions(...)`
*   `setTurnConstants(...)`
*   `setTurnExitConditions(...)`
*   `setHeadingConstants(...)`

**Example from `src/chassis.cpp`:**

```cpp
void resetChassis() {
  chassis.setHeading(inertial1.heading());
  chassis.stop(coast);

  chassis.setHeadingConstants(6, .4, 1);
  chassis.setTurnConstants(10, 0.2, .015, 1.5, 7.5);

  chassis.setDriveExitConditions(1, 200, 2000);
  chassis.setTurnExitConditions(1.5, 200, 1500);
}
```

## `PID` Class

The `PID` class implements a PID controller. It is used by the `Drive` class to control the robot's movement, but it can also be used independently.

### Initialization

The `PID` class has two constructors:

*   `PID(float error, float kp, float kd)`: Creates a PD controller.
*   `PID(float error, float kp, float ki, float kd, float starti, float settle_error, float settle_time, float timeout)`: Creates a PID controller.

### Usage

*   `compute(float error)`: Computes the PID output for a given error.
*   `isDone()`: Returns `true` if the PID controller has settled.

## `util` Functions

The `util.h` file contains several utility functions:

*   `reduce_0_to_360(float angle)`
*   `reduce_negative_180_to_180(float angle)`
*   `toDeg(float angle_rad)`
*   `threshold(float input, float min, float max)`
*   `toVolt(float percent)`
*   `deadband(float input, float width)`
*   `checkMotors(int motorCount, int temperatureLimit)`