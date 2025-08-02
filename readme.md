# RGB BOTS template

This project provides a lightweight C++ template for VEX V5 robotics competition, featuring a modular architecture, a library for autonomous driving control and companion web applications for robot remote control via typed or voice commands.

## Target Audience

This library is designed specifically for **middle school robotics teams** who are getting started with VEX V5 c++ programming and do not have odometry (tracking wheel) setups. Unlike advanced high school teams who use sophisticated libraries like [JAR-Template](https://github.com/JacksonAreaRobotics/JAR-Template) and [LemLib](https://github.com/LemLib/LemLib) that provide advanced features such as path following and pure pursuit, this template focuses on simplicity and ease of use. 

It provides essential driving controls, simple autonomous code structure, and straightforward configuration that allows teams to quickly get their robot operational without the complexity of advanced positioning systems. The built-in test mode allows teams to test all of their auton routines during driver control without having to re-download the program or use the field switch. The template provides a scaffold with sample code for the 2025-26 Push Back game, allowing teams to quickly extend and adapt the core functionality to their specific needs. Finally, the companion web applications for robot remote control via typed or voice commands add a new, fun, and interactive dimension to controlling a VEX robot. 

## Installation
*   **Prerequisites:** Before you begin, make sure:
    - You have [VSCode with VEX extension](https://kb.vex.com/hc/en-us/categories/360002333191-V5?sc=vs-code-extension) installed.
    - (Optional) You have the [GitHub](https://github.com/) account and extension in VS Code installed.
    - (Optional) If you are using Mac with Apple silicon: [install Rosetta](https://support.apple.com/en-us/102527) to simulate Intel chip if errors show up with VEX extension.
*   **Download and Open the Project:** 
    *   Download [the source code](https://github.com/ericjiangxiao/2026-base/archive/refs/heads/main.zip) as a ZIP file. Unzip and extract the downloaded file to a local folder.
    *   In VSCode, go to `File > Open Folder` and select the extracted folder.
    *  (Optional) Or follow the instructions to [pull the repository](doc/how_to_clone_and_pull_with_vscode.md) from github using VSCode.
*   **(Optional) If the project cannot build correctly**: 
    *   [Create a new v5 project](https://kb.vex.com/hc/en-us/articles/20146074601748-Creating-VS-Code-Projects-for-V5#:~:text=Select%20the%20'New%20Project'%20button,and%20select%20the%20corresponding%20icon.). Copy the dowloaded `src` and the `include` folders to the new project.

## Project Structure

The project is organized into the following directories:

*   `src/`: Main source code
    *   `main.cpp`: Entry point, competition control, and button mappings.
    *   `chassis.cpp`: Drivetrain configuration and control.
    *   `robot-config.cpp`: Configuration for other motors and sensors.
    *   `autons.cpp`: Autonomous routines.
    *   `rgb-template/`: Library code.
*   `include/`: Header files
*   `doc/`: Additional documentation.

## Configuration

For detailed step-by-step configuration instructions, see [Configuration Guide](doc/configuration_guide.md).

### Drivetrain ([chassis.cpp](src/chassis.cpp))

*   **Motors and Sensors:** Define the 6-motor drivetrain motors and inertial sensor, including ports, gear ratios, and motor direction. By default, the port numbers are 1, 2, 3 for the left side, 4, 5, 6 for the right side and 10 for the inertial sensor. 
*   **Drive Mode:** Set `DRIVE_MODE` to `0` for arcade control, `1` for tank control, or `2` for mecanum control.
*   **(Optional) Driver Control Constants:**
    *   `TURN_FACTOR`: Slows down the turning speed.
    *   `STEER_BIAS`: Controls the curve of the robot when both joysticks are used.
*   **(Optional) Wheel Size and Gear Ratio:**
    *   Find the Drive constructor and update the wheel diameter and gear ratio parameters for precise auton driving.
*   **(Optional) PID Constants:** If needed, adjust the PID constants for autonomous driving and turning in the `resetChassis()` function for auton driving. 

### Other Subsystems ([robot-config.cpp](src/robot-config.cpp))

*   **Motor Configuration:** Define your motors and sensors for other subsystems such as intake or lift.
*   **Number of Motors:** Set `NUMBER_OF_MOTORS` to total number of motors to allow the program to automatically check for disconnected or overheated motors.
*   **Helper functions:** Write helper functions to control the subsystems and declare those functions in [robot-config.h](include/robot-config.h) so that your other cpp files can call them.

### Driver Control ([main.cpp](src/main.cpp))

*   **Button Functions:** Create button functions by using the helper functions in  [robot-config.cpp](src/robot-config.cpp)
*   **Button Bindings:** At the bottom of the file, in the `main()` function, map controller buttons to the functions.

### Autonomous Routines ([autons.cpp](src/autons.cpp))

*   **Auton Functions:** Write your autonomous routines as separate functions.
*   **Auton Mappings:** Map auton functions to menu items in the `runAutonItem()` function 
*   **Auton Menu Text:** Add the names of your autonomous functions to the `autonMenuText` array to make them shown on the brain's and controller's screen.

## Run sample program
- Build and download the program to the brain and use game controller to select the program slot and run the program.
- Test the selected drive mode:
  - **Arcade Drive (Mode 0)**: Use left stick to turn and right stick to drive forward/backward
  - **Tank Drive (Mode 1)**: Use left stick for left side motors, right stick for right side motors  
  - **Mecanum Drive (Mode 2)**: Use left stick for forward/backward and turning, right stick for strafing
- If you press the `B button`, the controller shows the current heading and the distance driven and resets the motor encoder. For sample button mappings and actions, see [button controls](doc/button_control.md).
- (Optional) Test all autons during driver control using the game controller:
    - Press the controller's `Right button` within 5 seconds of program startup to enter test mode.
    - When in test mode, press the `Right button` to cycle through the list of autonomous routines on the controller screen.
    - Press the `A button` to run the routine shown on the screen. Hold the button to continue past stops.
    - See the complete action flow in [Test Auton Button Flow Explanation](doc/test_auton_buttons.md)
- (Optional) Remotely control the robot in browser to design and test auton routes without rebuilding programs:
    - Read [remote control document](/voice-control-robot/README.md) for the steps and basic understanding how it works.
&nbsp;
---
# More on the Programming Interfaces of the [Library](src/rgb-template/)
## Drive APIs ([drive.h](include/rgb-template/drive.h))

The `Drive` class provides a set of APIs to control the robot's movement.

### `driveWithVoltage(float leftVoltage, float rightVoltage)`

This is the most basic drive function. It sets the voltage for the left and right sides of the drivetrain directly.

**Example:**

```cpp
// Drive forward at half speed for 1 second
chassis.driveWithVoltage(6, 6);
wait(1000, msec);
chassis.stop(coast);
```

### `turnToHeading(...)`

This API turns the robot to a specific heading using a PID controller.

**Variations:**

1.  `turnToHeading(float heading)`: Turns to the specified heading with default parameters.
2.  `turnToHeading(float heading, float turnMaxVoltage)`: Limits the maximum voltage for a slower, more accurate turn.
3.  `turnToHeading(float heading, float turnMaxVoltage, bool chaining, float settleError = 5, float settleTime = 50)`: Advanced version for faster turns with optional non-blocking execution.

**Examples:**

```cpp
// Simple turn to 90 degrees
chassis.turnToHeading(90);

// Slower, more accurate turn
chassis.turnToHeading(90, 6);

// Fast turn without waiting for completion
chassis.turnToHeading(90, 12, true);
```

### `driveDistance(...)`

This API drives the robot a specific distance using a PID controller, with options for maintaining a heading.

**Variations:**

1.  `driveDistance(float distance)`: Drives the specified distance with default parameters.
2.  `driveDistance(float distance, float driveMaxVoltage)`: Limits the maximum voltage for driving.
3.  `driveDistance(float distance, float driveMaxVoltage, float heading, float headingMaxVoltage)`: Drives while maintaining a specific heading (curved drive).
4.  `driveDistance(float distance, float driveMaxVoltage, float heading, float headingMaxVoltage, bool chaining, float driveSettleError=2, float driveSettleTime=50)`: Advanced version for curved driving with optional non-blocking execution.

**Examples:**

```cpp
// Drive forward 24 inches
chassis.driveDistance(24);

// Drive forward 24 inches with a maximum voltage of 8
chassis.driveDistance(24, 8);

// Drive forward 24 inches while turning to a heading of 45 degrees
chassis.driveDistance(24, 10, 45, 6);
```

## PID Control ([PID.h](include/rgb-template/PID.h))

This class provides two PID controller constructors:

1.  `PID(float error, float kp, float kd)`: A simple PID controller for applications like heading correction, where integral control is not necessary.
2.  `PID(float error, float kp, float ki, float kd, float starti, float settle_error, float settle_time, float timeout)`: A full PID controller for more complex systems like arms or lifts, where precise control is required.