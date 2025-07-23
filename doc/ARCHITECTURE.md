# Project Architecture

## Overall Structure

This is a standard VEX V5 project that follows a typical structure for a competition robot. It's organized into a few main directories:

*   **`include`**: Contains header files (`.h`) that define the interfaces for different robot modules.
*   **`src`**: Contains the corresponding implementation files (`.cpp`) for the headers in `include`, as well as the main program logic.
*   **`vex`**: Contains makefiles for building the project.
*   **`GEMINI.md`**: Contains project information.

## Modules

The code is modularized into the following components:

*   **`main.cpp`**: This is the entry point of the program. It sets up the competition functions for autonomous and driver control modes, and it maps controller buttons to specific actions.
*   **`chassis.h` / `chassis.cpp`**: This module likely encapsulates the robot's drivetrain logic. It provides functions for user control (`usercontrol`) and autonomous control (`autonomous`). It also appears to have a `Drive` class that represents the chassis.
*   **`autons.h` / `autons.cpp`**: This module manages the autonomous routines. It provides a menu system (`show_auton_menu`) to select different autonomous programs, and it has functions to execute the selected autonomous routine (`run_auton_item`, `run_auton_test`).
*   **`robot-config.h` / `robot-config.cpp`**: This module is responsible for configuring the robot's hardware components, such as motors and sensors. It defines and initializes the `intake` motor, `clampMotor`, `clampRotation` sensor, and `mogo_distance` sensor. It also provides functions to control these components (e.g., `clamp_mogo`, `release_mogo`).
*   **`rgb-template`**: This subdirectory contains a reusable library for controlling a drivetrain. It includes the following components:
    *   **`drive.h` / `drive.cpp`**: This defines a `Drive` class that encapsulates the robot's drivetrain. It provides methods for controlling the robot's movement, including turning to a specific heading and driving a specific distance. It also includes logic for driver control with arcade and tank modes.
    *   **`PID.h` / `PID.cpp`**: This implements a PID (Proportional-Integral-Derivative) controller, which is a common control loop feedback mechanism used in robotics to for precise movements.
    *   **`util.h` / `util.cpp`**: This contains various utility functions, such as for angle conversions and motor checks.

## Control Flow

1.  **`main()`**: The `main` function in `main.cpp` is the starting point.
2.  **`pre_auton()`**: Before the match begins, the `pre_auton` function is called. This is where the autonomous selection menu is shown.
3.  **Competition Modes**: The VEX Competition switch determines whether the robot enters the `autonomous` or `usercontrol` function.
    *   **`autonomous()`**: This function, likely defined in `chassis.cpp`, executes the selected autonomous routine from the `autons` module.
    *   **`usercontrol()`**: This function, also likely in `chassis.cpp`, handles driver control, translating controller input into robot actions. It also calls other functions based on button presses, such as those for the intake and clamp.

In summary, this project has a well-defined architecture that separates concerns into distinct modules. This makes the code easier to understand, maintain, and extend. The use of a makefile-based build system is also a standard practice for this type of project.


