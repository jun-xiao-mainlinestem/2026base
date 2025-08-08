# Project Architecture

## Overall Structure

This is a standard VEX V5 project that follows a typical structure for a competition robot. It's organized into a few main directories:

*   **`include`**: Contains header files (`.h`) that define the interfaces for different robot modules.
*   **`src`**: Contains the corresponding implementation files (`.cpp`) for the headers in `include`, as well as the main program logic.
*   **`vex`**: Contains makefiles for building the project.
*   **`doc`**: Contains project documentation and implementation guides.

## Modules

The code is modularized into the following components:

*   **`main.cpp`**: This is the entry point of the program. It sets up the competition functions for autonomous and driver control modes, and it maps controller buttons to specific actions. It also integrates the SampleRemoteControl module for command processing.

*   **`autons.h` / `autons.cpp`**: This module manages the autonomous routines. It provides a menu system (`showAutonMenu`) to select different autonomous programs, and it has functions to execute the selected autonomous routine (`runAutonItem`).

*   **`robot-config.h` / `robot-config.cpp`**: This module is responsible for configuring the robot's hardware components, such as motors and sensors. It defines and initializes the `chassis` and `intake` motor etc. It also provides functions to control these components.

*   **`rgb-template`**: This subdirectory contains a reusable library for controlling a drivetrain and remote communication. It includes the following components:
    *   **`drive.h` / `drive.cpp`**: This defines a `Drive` class that encapsulates the robot's drivetrain. It provides methods for controlling the robot's movement, including turning to a specific heading and driving a specific distance. It also includes logic for driver control with arcade, tank and mecanum modes.
    *   **`PID.h` / `PID.cpp`**: This implements a PID (Proportional-Integral-Derivative) controller, which is a common control loop feedback mechanism used in robotics for precise movements.
    *   **`util.h` / `util.cpp`**: This contains various utility functions, such as for angle conversions and motor checks.
*   **`SampleRemoteControl.h` / `SampleRemoteControl.cpp`**: This implements serial communication for voice control via the SampleRemoteControl class. It handles reading commands from the USB serial port, processing voice commands, and executing robot actions. It also manages status reporting back to the web interface.

## Web Application Integration

### SampleRemoteControl Module
The SampleRemoteControl module is a key component that enables voice control of the robot:

#### Key Features
- **Serial Communication**: Reads commands from `/dev/serial1` via file-based I/O
- **Command Processing**: Parses complete word commands (FORWARD, STOP, LEFT, etc.)
- **Status Reporting**: Sends robot status (heading, distance) back to web interface
- **Error Handling**: Graceful handling of communication failures and invalid commands

#### Integration Points
- **Main Loop**: Polled every 100ms when remote control is enabled
- **Robot Actions**: Direct control of chassis and mechanisms

#### Communication Flow
```
Web Interface → WebSocket → VEX Extension → Serial → SampleRemoteControl → Robot Actions
```

## Control Flow

1.  **`main()`**: The `main` function in `main.cpp` is the starting point.
2.  **`pre_auton()`**: Before the match begins, the `pre_auton` function is called. This is where the autonomous selection menu is shown.
3.  **Competition Modes**: The VEX Competition switch determines whether the robot enters the `autonomous` or `usercontrol` function.
    *   **`autonomous()`**: This function, likely defined in `chassis.cpp`, executes the selected autonomous routine from the `autons` module.
    *   **`usercontrol()`**: This function, also likely in `chassis.cpp`, handles driver control, translating controller input into robot actions. It also calls other functions based on button presses, such as those for the intake and clamp.
4.  **Remote Control Loop**: If enabled, the main loop continuously polls the SampleRemoteControl module for new remote commands.


