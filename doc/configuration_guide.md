# Configuration Guide

This guide provides step-by-step instructions for configuring your VEX V5 robot code using RGB template.

**⚠️ Important:** Only the **Drivetrain Configuration** section is mandatory for you to be able to test drive. The other sections are optional and depend on your specific needs.

## Table of Contents
1. [Drivetrain Configuration](#drivetrain-configuration) *(Required)*
2. [Subsystem Configuration](#motor-and-sensor-configuration) *(Optional)*
3. [Button Control Configuration](#driver-control-configuration) *(Optional)*
4. [Autonomous Routines Configuration](#autonomous-routines-configuration) *(Optional)*

---

## Drivetrain Configuration *(Required)*

This section is **mandatory** - you must configure your drivetrain for the robot to function properly.

### Step 1: Open chassis.cpp
Navigate to `src/chassis.cpp` and locate the motor definitions.

### Step 2: Configure Motor Ports
Find the motor definitions and update the port numbers:

```cpp
// Left side motors
motor left_front = motor(PORT1, ratio18_1, false);
motor left_middle = motor(PORT2, ratio18_1, false);
motor left_back = motor(PORT3, ratio18_1, false);

// Right side motors
motor right_front = motor(PORT4, ratio18_1, true);
motor right_middle = motor(PORT5, ratio18_1, true);
motor right_back = motor(PORT6, ratio18_1, true);

// Inertial sensor
inertial inertial_sensor = inertial(PORT10);
```

**Action:** Replace `PORT1`, `PORT2`, etc. with your actual motor port numbers.

### Step 3: Set Drive Mode
Locate the drive mode setting:

```cpp
int DRIVE_MODE = 0;  // 0 for arcade, 1 for tank, 2 for mecanum
```

**Action:** Set the drive mode value:
- `0` for arcade drive (single joystick for movement and turning)
- `1` for tank drive (left joystick for left side, right joystick for right side)
- `2` for mecanum drive (four-wheel independent control for strafing)

### Step 4: Configure Drive Constants
Find the driver control constants:

```cpp
float TURN_FACTOR = 0.75;  // Slows down turning speed
float STEER_BIAS = 0.5;   // Controls curve when both joysticks used
```

**Action:** Adjust these values based on your driver's preference.

### Step 5: Configure Wheel Size and Gear Ratio
Find the Drive constructor and update the wheel diameter and gear ratio parameters:

```cpp
Drive chassis(
  //Left Motors:
  motor_group(leftMotor1, leftMotor2, leftMotor3),
  //Right Motors:
  motor_group(rightMotor1, rightMotor2, rightMotor3),
  //Inertial Sensor:
  inertial1,
  //wheel diameter:
  2.75,
  //Gear ratio of motor to wheel: if your motor has an 36-tooth gear and your wheel has a 48-tooth gear, this value will be 0.75. For direct drive, this value will be 1.
  0.75
);
```

**Action:** 
- Set the wheel diameter (currently `2.75`) to your actual wheel diameter in inches
- Set the gear ratio (currently `0.75`) to your motor-to-wheel gear ratio:
  - `1.0` for direct drive (motor directly connected to wheel)
  - `0.75` for 36:48 gear ratio (36-tooth motor gear, 48-tooth wheel gear)
  - `2.0` for 2:1 gearbox (2 motor rotations = 1 wheel rotation)
  - `0.5` for 1:2 gearbox (0.5 motor rotations = 1 wheel rotation)

---

## Subsystem Configuration *(Optional)*

This section is **optional** - you only need to configure this if you have motors or sensors other than the drivetrain.

### Step 1: Open robot-config.cpp
Navigate to `src/robot-config.cpp` and locate the motor definitions.

### Step 2: Configure Subsystem Motors
Find the motor definitions for your subsystems:

```cpp
// Intake motors
motor intake_motor = motor(PORT7, ratio18_1, false);
// Other motors as needed
```

**Action:** Add your motors with correct port numbers and gear ratios.

### Step 3: Set Total Motor Count
Locate the motor count definition:

```cpp
int NUMBER_OF_MOTORS = 8;  // Total number of motors
```

**Action:** Update to match your total number of motors.

### Step 4: Create Helper Functions
Add helper functions for your subsystems:

```cpp
void inTake() {
    intakeMotor.spin(forward, 100, percent);
}

void stopRollers() {
    intakeMotor.stop();
}
```

**Action:** Create functions for each subsystem you want to control.

### Step 5: Declare Functions in Header
Open `include/robot-config.h` and add function declarations:

```cpp
void inTake();
void stopRollers();
// Add other function declarations
```

---

## Button Control Configuration *(Optional)*

This section is **optional** - you only need to configure this if you have button control logic.

### Step 1: Open main.cpp
Navigate to `src/main.cpp` and locate the button functions.

### Step 2: Create Button Functions
Add button functions for your subsystems:

```cpp
void buttonL1Action() {
    inTake();
    while(controller(primary).ButtonL1.pressing()) {
        wait(20, msec);
    }
    stopRollers();
}

```

**Action:** Create button functions for each action you want to control.

### Step 3: Map Buttons to Functions
Find the button mapping section in the `main()` function:

```cpp
controller(primary).ButtonL1.pressed(buttonL1Action);
```

**Action:** Map your button functions to controller buttons.

---

## Autonomous Routines Configuration *(Optional)*

This section is **optional** - you only need to configure this if you have autonomous routines.

### Step 1: Open autons.cpp
Navigate to `src/autons.cpp` and locate the auton functions.

### Step 2: Create Auton Functions
Add your autonomous routines:

```cpp
void auton1() {
    // Drive forward 24 inches
    chassis.driveDistance(24);
}

void auton2() {
    // Simple forward movement
    chassis.driveWithVoltage(6, 6);
    wait(1000, msec);
    chassis.stop(brake);
}
```

**Action:** Write your autonomous routines using the drive APIs.

### Step 3: Map Auton Functions
Find the `runAutonItem()` function:

```cpp
void runAutonItem() {
    switch (currentAutonSelection) {
    case 0:
        auton1();
        break;
    case 1:
        auton2();
        break;
    }
}
```

**Action:** Add your auton functions to the switch statement.

### Step 4: Add Menu Text
Find the `auton_menu_text` array:

```cpp
char const * auton_menu_text[] = {
    "auton1",
    "auton2"
};
```

**Action:** Add names for your autonomous routines.

---

## Testing Your Configuration

### Step 1: Build the Project
1. Open the project in VSCode
2. Press `Ctrl+Shift+P` (or `Cmd+Shift+P` on Mac)
3. Type "VEX: Build" and select it
4. Check for any compilation errors

### Step 2: Download to Robot
1. Connect your VEX brain via USB
2. Press `Ctrl+Shift+P` (or `Cmd+Shift+P` on Mac)
3. Type "VEX: Download" and select it
4. Wait for the download to complete

### Step 3: Test Driver Control
1. Run the program on the brain
2. Test the selected drive mode:
   - **Arcade Drive (Mode 0)**: Use left stick to turn and right stick to drive forward/backward
   - **Tank Drive (Mode 1)**: Use left stick for left side motors, right stick for right side motors
   - **Mecanum Drive (Mode 2)**: Use left stick for forward/backward and turning, right stick for strafing
3. Verify motors respond correctly


---

## Troubleshooting

### Common Issues:
1. **Motors not responding or in conflict**: Check port numbers and motor directions
2. **Robot drives in wrong direction**: Reverse motor directions in chassis.cpp
3. **Buttons not working**: Verify button mappings in main.cpp
4. **Auton not running**: Check auton function names in runAutonItem()

### Debug Tips:
1. Use the controller screen to display debug information
2. Add `controller(primary).Screen.print()` statements
3. Check motor temperatures and connections

For more detailed explanation of the APIs, refer to the main [README.md](/readme.md) file. 