# RGB BOTS template
By Haverford Robotics Team 169 (The Cavalry) and RGB Bots Team 99009A (the little red pony).

This project provides a lightweight C++ template for VEX V5 robotics competition and beyond, featuring a modular architecture, a library for autonomous driving control and a companion web application for robot remote driving via form or speech input.

## Target Audience

This library is designed specifically for **middle school robotics teams** who are getting started with VEX V5 c++ programming and do not have odometry (tracking wheel) setups. Advanced high school teams often use comprehensive libraries like [JAR-Template](https://github.com/JacksonAreaRobotics/JAR-Template) and [LemLib](https://github.com/LemLib/LemLib) that provide features such as path following and pure pursuit. In contrast, this template focuses on simplicity and ease of use, allows teams to quickly get their robot operational without the complexity of advanced positioning systems.

It provides essential PID driving controls, a simple autonomous code structure, and a straightforward robot configuration that allows teams to quickly extend and adapt the core functionality to their specific needs. The built-in test mode allows teams to step through and test all of their autonomous routines during driver control, providing an alternative way to debug and iterate on their logic without having to re-download the program or use the field switch. Finally, the companion web application for robot remote driving via form or speech input on mobile devices add a fun and interactive dimension to controlling a VEX robot beyond V5RC competitions.

## Installation
*   **Prerequisites:** Before you begin, make sure:
    - You have [VSCode with VEX extension](https://kb.vex.com/hc/en-us/categories/360002333191-V5?sc=vs-code-extension) installed.
    - (Optional) You have the [GitHub](https://github.com/) account and extension in VS Code installed.
    - (Optional) If you are using Mac with Apple silicon: [install Rosetta](https://support.apple.com/en-us/102527) to simulate Intel chip if errors show up with VEX extension.
*   **Download and Open the Project:** 
    *   Download [the source code]() as a ZIP file. Unzip and extract the downloaded file to a local folder.
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
*   **(Optional) Wheel Size and Gear Ratio:**
    *   Find the Drive constructor and update the wheel diameter and gear ratio parameters for precise auton driving.
*   **(Optional) Drive Constants:** If needed, adjust any of constants for the drivetrain in the `setChassisDefaults()` function. For example, adjust the `kTurnDampingFactor` value in `setArcadeConstants()` to control turn sensitivity - lower values make turning less sensitive, higher values make turning more sensitive. 

### Other Subsystems ([robot-config.cpp](src/robot-config.cpp))

*   **Motor Configuration:** Define your motors and sensors for other subsystems such as intake or lift.
*   **Number of Motors:** Set `NUMBER_OF_MOTORS` to total number of motors to allow the program to automatically check for disconnected or overheated motors. The controller will vibrate with "---" pattern and display warning messages if any motor issues are detected.
*   **Helper functions:** Write helper functions to control the subsystems and declare those functions in [robot-config.h](include/robot-config.h) so that your other cpp files can call them.

### Driver Control ([main.cpp](src/main.cpp))

*   **Button Functions:** Create button functions by using the helper functions in  [robot-config.cpp](src/robot-config.cpp)
*   **Button Bindings:** In the `setupButtonMapping()` function, map controller buttons to the functions.

### Autonomous Routines ([autons.cpp](src/autons.cpp))

*   **Auton Functions:** Write your autonomous routines as separate functions.
*   **Auton Mappings:** Map auton functions to menu items in the `runAutonItem()` function 
*   **Auton Menu Text:** Add the names of your autonomous functions to the `autonMenuText` array to make them shown on the brain's and controller's screen.

## Run sample program
- Build and download the program to the brain and use game controller to select the program slot and run the program.
- Drive mode:
  - Arcade Drive (default): Use left stick to turn and right stick to drive forward/backward
  - Tank Drive: Use left stick for left side motors, right stick for right side motors  
  - Mecanum Drive: Use left stick for forward/backward and turning, right stick for strafing
  - Change drive mode during driver control: Press the controller's `B button` within 5 seconds of program startup to switch from tank to arcade drive mode and vice versa.
- Run individual auton:
  - Set default auton: Set the `currentAutonSelection` value in `auton.cpp` and run `timed run`.
  - Auton Selection:  Before autonomous period is started via the field switch, press the brain screen to scroll through the list of autons.
- (Optional) Test all autons during driver control:
    - Press the controller's `Right button` within 5 seconds of program startup to enter test mode.
    - When in test mode, press the `A button` to run the selected auton or current step.
    - When in test mode, press the `Right/Left buttons` to cycle through the list of autonomous routines.
    - When in test mode, press the `Up/Down buttons` to navigate through individual steps of the current auton.
    - See the complete action flow in [Test Auton Button Flow Explanation](doc/test_auton_buttons.md)
- (Optional) Remotely control the robot with the companion web application:
    - Enable Websocket Server in VS Code VEX Extension 
    - Connect the controller to the computer via USB and download the program to the brain.
    - Find the device ID of the brain and the ip address of the computer.
    - Open the website on your mobile device (on the same wifi network).
    - For detailed step-by-step setup instructions, see [setup Guide](doc/remote_setup_guide.md).
- **Motor Health Monitoring**: The controller will vibrate with "---" pattern and display warning messages if any motors are disconnected or overheated (temperature limit: 45°C). Check motor connections and temperatures immediately when alerts occur.
   
&nbsp;
---
# Programming Interfaces of the library

## Voltage Limits
**Important:** The VEX V5 brain operates on a 12V system. All voltage-based functions have a maximum voltage of 12V.

- **Maximum voltage**: 12V
- **Recommended range**: 2V to 10V for precise control
- **Full speed**: 12V (use sparingly for fine movements)
- **Half speed**: 6V (good for most movements)
- **Low speed**: 2-4V (for precise positioning)
## Drive APIs ([drive.h](include/rgb-template/drive.h))

The `Drive` class provides a set of APIs to control the robot's movement.

### `driveWithVoltage(float leftVoltage, float rightVoltage)`

This is the most basic drive function. It sets the voltage for the left and right sides of the drivetrain directly. **Maximum voltage is 12V.**

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
2.  `turnToHeading(float heading, float turnMaxVoltage)`: Limits the maximum voltage for a slower, more accurate turn. **Maximum voltage is 12V.**

**Examples:**

```cpp
// Simple turn to 90 degrees
chassis.turnToHeading(90);

// Slower, more accurate turn
chassis.turnToHeading(90, 6);
```

### `driveDistance(...)`

This API drives the robot a specific distance using a PID controller, with options for maintaining a heading.

**Variations:**

1.  `driveDistance(float distance)`: Drives the specified distance with default parameters.
2.  `driveDistance(float distance, float driveMaxVoltage)`: Limits the maximum voltage for driving. **Maximum voltage is 12V.**
3.  `driveDistance(float distance, float driveMaxVoltage, float heading, float headingMaxVoltage)`: Drives while turning a specific heading (curved drive). **Maximum voltage is 12V.**

**Examples:**

```cpp
// Drive forward 24 inches
chassis.driveDistance(24);

// Drive forward 24 inches with a maximum voltage of 8
chassis.driveDistance(24, 8);

// Drive forward 24 inches while turning to a heading of 45 degrees
chassis.driveDistance(24, 10, 45, 6);
```

### `setHeading(...)`

This API set the robot to a specific heading.

**Examples:**

```cpp
// set robot intial heading to 90 degrees
chassis.setHeading(90);

```