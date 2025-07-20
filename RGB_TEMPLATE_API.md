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

#### `turn_to_heading`

This function turns the robot to a specific heading in degrees.

*   `turn_to_heading(float heading)`: Turns the robot to a specific heading with default parameters.
*   `turn_to_heading(float heading, float turn_max_voltage)`: Turns the robot to a specific heading with a specified maximum voltage.
*   `turn_to_heading(float heading, float turn_max_voltage, bool nonstop, ...)`: Turns the robot to a specific heading with more control over the movement.

**Examples from `src/autons.cpp`:**

```cpp
// Simple turn
chassis.turn_to_heading(90);

// Turn with specified max voltage and nonstop flag
chassis.turn_to_heading(180, 10, true);
```

#### `drive_distance`

This function drives the robot a specific distance in inches.

*   `drive_distance(float distance)`: Drives a specific distance with default parameters.
*   `drive_distance(float distance, float drive_max_voltage)`: Drives a specific distance with a specified maximum voltage.
*   `drive_distance(float distance, float drive_max_voltage, float heading, float heading_max_voltage)`: Drives a specific distance while maintaining a specific heading.
*   `drive_distance(float distance, float drive_max_voltage, float heading, float heading_max_voltage, bool nonstop, ...)`: Drives a specific distance with more control over the movement.

**Examples from `src/autons.cpp`:**

```cpp
// Drive a specific distance with a timeout
chassis.drive_distance(24);

// Drive a specific distance with a timeout and heading
chassis.drive_distance(24, 10, 0, 6, true);
```

### Driver Control

The `Drive` class also provides methods for controlling the robot during the driver control period.

*   `control_arcade(int throttle, int turn, float turnBias)`: Controls the robot using arcade-style controls.
*   `control_tank(int left, int right)`: Controls the robot using tank-style controls.

**Example from `src/chassis.cpp`:**

```cpp
void usercontrol(void) {
  // ...
  while (1) {
    if (DRIVE_TANK_MODE) chassis.control_tank(controller(primary).Axis3.position(), controller(primary).Axis2.position());
    else {
        chassis.control_arcade(controller(primary).Axis2.position(), controller(primary).Axis4.position());
    }
    wait(20, msec); 
  }
}
```

### Configuration

The `Drive` class provides several methods for configuring the drivetrain's PID constants and exit conditions.

*   `set_drive_constants(...)`
*   `set_drive_exit_conditions(...)`
*   `set_turn_constants(...)`
*   `set_turn_exit_conditions(...)`
*   `set_heading_constants(...)`

**Example from `src/chassis.cpp`:**

```cpp
void reset_chassis() {
  chassis.set_heading(inertial1.heading());
  chassis.stop(coast);

  chassis.set_drive_constants(10, 1.5, 0, 10, 0);
  chassis.set_heading_constants(6, .4, 1);
  chassis.set_turn_constants(10, 0.2, .015, 1.5, 7.5);

  chassis.set_drive_exit_conditions(1, 200, 2000);
  chassis.set_turn_exit_conditions(1.5, 200, 1500);
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
*   `is_done()`: Returns `true` if the PID controller has settled.

## `util` Functions

The `util.h` file contains several utility functions:

*   `reduce_0_to_360(float angle)`
*   `reduce_negative_180_to_180(float angle)`
*   `to_deg(float angle_rad)`
*   `threshold(float input, float min, float max)`
*   `to_volt(float percent)`
*   `deadband(float input, float width)`
*   `check_motors(int motor_count, int temperature_limit)`