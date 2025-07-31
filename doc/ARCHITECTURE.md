# Project Architecture

## Overall Structure

This is a standard VEX V5 project that follows a typical structure for a competition robot. It's organized into a few main directories:

*   **`include`**: Contains header files (`.h`) that define the interfaces for different robot modules.
*   **`src`**: Contains the corresponding implementation files (`.cpp`) for the headers in `include`, as well as the main program logic.
*   **`vex`**: Contains makefiles for building the project.
*   **`doc`**: Contains project documentation and implementation guides.

## Modules

The code is modularized into the following components:

*   **`main.cpp`**: This is the entry point of the program. It sets up the competition functions for autonomous and driver control modes, and it maps controller buttons to specific actions. It also integrates the RemoteControl module for voice command processing.

*   **`chassis.h` / `chassis.cpp`**: This module encapsulates the robot's drivetrain logic. It provides functions for user control (`usercontrol`) and autonomous control (`autonomous`). It also has a `Drive` class that represents the chassis.

*   **`autons.h` / `autons.cpp`**: This module manages the autonomous routines. It provides a menu system (`showAutonMenu`) to select different autonomous programs, and it has functions to execute the selected autonomous routine (`runAutonItem`, `runAutonTest`).

*   **`robot-config.h` / `robot-config.cpp`**: This module is responsible for configuring the robot's hardware components, such as motors and sensors. It defines and initializes the `intake` motor, `clampMotor`, `clampRotation` sensor, and `mogoDistance` sensor. It also provides functions to control these components (e.g., `clampMogo`, `releaseMogo`).

*   **`rgb-template`**: This subdirectory contains a reusable library for controlling a drivetrain and remote communication. It includes the following components:
    *   **`drive.h` / `drive.cpp`**: This defines a `Drive` class that encapsulates the robot's drivetrain. It provides methods for controlling the robot's movement, including turning to a specific heading and driving a specific distance. It also includes logic for driver control with arcade and tank modes.
    *   **`PID.h` / `PID.cpp`**: This implements a PID (Proportional-Integral-Derivative) controller, which is a common control loop feedback mechanism used in robotics for precise movements.
    *   **`util.h` / `util.cpp`**: This contains various utility functions, such as for angle conversions and motor checks.
    *   **`RemoteControl.h` / `RemoteControl.cpp`**: This implements serial communication for voice control. It handles reading commands from the USB serial port, processing voice commands, and executing robot actions. It also manages status reporting back to the web interface.

## Voice Control Integration

### RemoteControl Module
The RemoteControl module is a key component that enables voice control of the robot:

#### Key Features
- **Serial Communication**: Reads commands from `/dev/serial1` via file-based I/O
- **Command Processing**: Parses complete word commands (FORWARD, STOP, LEFT, etc.)
- **Status Reporting**: Sends robot status (heading, distance) back to web interface
- **Error Handling**: Graceful handling of communication failures and invalid commands

#### Integration Points
- **Main Loop**: Polled every 100ms when voice control is enabled
- **Button Control**: Toggle via Button X on controller
- **Robot Actions**: Direct control of chassis and mechanisms
- **Debug Output**: Controller screen display and rumble feedback

#### Communication Flow
```
Web Interface → WebSocket → VEX Extension → Serial → RemoteControl → Robot Actions
```

## Control Flow

1.  **`main()`**: The `main` function in `main.cpp` is the starting point.
2.  **`pre_auton()`**: Before the match begins, the `pre_auton` function is called. This is where the autonomous selection menu is shown.
3.  **Competition Modes**: The VEX Competition switch determines whether the robot enters the `autonomous` or `usercontrol` function.
    *   **`autonomous()`**: This function, likely defined in `chassis.cpp`, executes the selected autonomous routine from the `autons` module.
    *   **`usercontrol()`**: This function, also likely in `chassis.cpp`, handles driver control, translating controller input into robot actions. It also calls other functions based on button presses, such as those for the intake and clamp.
4.  **Voice Control Loop**: When enabled via Button X, the main loop continuously polls the RemoteControl module for new voice commands.

## Error Handling and Debugging

### Controller Feedback
- **Screen display**: Shows received commands and processing status
- **Rumble patterns**: Different patterns for different states
- **Error messages**: Clear indication of communication issues

### Robust Operation
- **Non-blocking**: Serial operations don't interfere with robot control
- **Graceful failures**: Communication errors don't crash the system
- **Recovery**: Automatic reconnection and error recovery
- **Fallbacks**: Manual control remains available during voice control

## Summary

This project has a well-defined architecture that separates concerns into distinct modules. The addition of the RemoteControl module provides voice control capabilities while maintaining the existing autonomous and driver control functionality. The use of a makefile-based build system is also a standard practice for this type of project.

The modular design makes the code easier to understand, maintain, and extend. The voice control integration demonstrates how modern web technologies can be effectively combined with VEX robotics for enhanced user experience.


