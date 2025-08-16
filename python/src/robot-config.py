#!/usr/bin/env python3
"""
VEX Robot Configuration - Python Version (Simplified)
Motor and sensor definitions and helper functions
"""

from vex import *
from rgb_template.drive import Drive
from vex import wait, MSEC, BRAKE, FORWARD, VoltageUnits, DEGREES, HOLD, COAST, DEGREES

# ------------------------------------------------------------------------
#              Drivetrain definition
# ------------------------------------------------------------------------
# If you only have 4 motors (or mecanum drive), assign leftMotor3, rightMotor3 to unused ports.
left_motor1 = Motor(Ports.PORT1, GearSetting.RATIO_6_1, True)
left_motor2 = Motor(Ports.PORT2, GearSetting.RATIO_6_1, True)
left_motor3 = Motor(Ports.PORT3, GearSetting.RATIO_18_1, False)

right_motor1 = Motor(Ports.PORT4, GearSetting.RATIO_6_1, False)
right_motor2 = Motor(Ports.PORT5, GearSetting.RATIO_6_1, False)
right_motor3 = Motor(Ports.PORT6, GearSetting.RATIO_18_1, True)

# Create motor groups for left and right sides
left_drive = MotorGroup(left_motor1, left_motor2, left_motor3)
right_drive = MotorGroup(right_motor1, right_motor2, right_motor3)

# Inertial sensor for auton turning and heading
# If you do not have an inertial sensor, assign it to an unused port. Ignore the warning at the start of the program.
inertial1 = Inertial(Ports.PORT10)

# 0: double arcade drive, 1: single arcade, 2: tank drive, 3: mecanum drive
DRIVE_MODE = 0

# ------------------------------------------------------------------------
#        Other subsystems: motors, sensors and helper functions definition
# ------------------------------------------------------------------------
roller = Motor(Ports.PORT11, GearSetting.RATIO_18_1, True)

# Total number of motors, including drivetrain
NUMBER_OF_MOTORS = 7

def in_take():
    """Intake function - runs the intake rollers"""
    roller.spin(FORWARD, 12, VoltageUnits.VOLT)

def out_take():
    """Outtake function - reverses the intake rollers"""
    roller.spin(FORWARD, -12, VoltageUnits.VOLT)

def stop_rollers():
    """Stops the roller motors"""
    roller.stop(BRAKE)

# ------------------------------------------------------------------------
#               Only change code below this line when necessary
# ------------------------------------------------------------------------

# A global instance of brain used for printing to the V5 Brain screen
brain = Brain()
controller1 = Controller(PRIMARY)

# Create chassis instance
chassis = Drive(
    # Left Motors
    left_drive,
    # Right Motors
    right_drive,
    # Inertial Sensor
    inertial1,
    # Wheel diameter
    2.75,
    # Gear ratio of motor to wheel: if your motor has a 36-tooth gear and your wheel has a 48-tooth gear, this value will be 0.75.
    0.75
)

def set_chassis_defaults():
    """Resets the chassis constants"""
    # Sets the heading of the chassis to the current heading of the inertial sensor
    chassis.set_heading(chassis.gyro.heading(DEGREES))
    
    # Sets the drive PID constants for the chassis
    # These constants are used to control the acceleration and deceleration of the chassis
    chassis.set_drive_pid(10, 1.5, 0, 10, 0)
    
    # Sets the turn PID constants for the chassis
    # These constants are used to control the turning of the chassis
    chassis.set_turn_pid(10, 0.2, 0.015, 1.5, 7.5)
    
    # Sets the heading PID constants for the chassis
    # These constants are used to control the heading adjustment of the chassis
    chassis.set_heading_pid(6, 0.4, 1)
    
    # Sets the exit conditions for the drive functions
    # These conditions are used to determine when the drive function should exit
    chassis.set_drive_exit_conditions(1, 200, 2000)
    
    # Sets the exit conditions for the turn functions
    # These conditions are used to determine when the turn function should exit
    chassis.set_turn_exit_conditions(1.5, 200, 1500)
    
    # Sets the arcade drive constants for the chassis
    # These constants are used to control the arcade drive of the chassis
    chassis.set_arcade_constants(0.5, 0.5, 0.85)

def change_drive_mode():
    """Change the drive mode"""
    controller1.rumble("-")
    global DRIVE_MODE
    DRIVE_MODE = (DRIVE_MODE + 1) % 3
    
    if DRIVE_MODE == 0:
        brain.screen.print("Double Arcade")
    elif DRIVE_MODE == 1:
        brain.screen.print("Single Arcade")
    elif DRIVE_MODE == 2:
        brain.screen.print("Tank Drive")
    elif DRIVE_MODE == 3:
        brain.screen.print("Mecanum Drive")

def exit_auton():
    """Exit the autonomous menu"""
    # This function handles exiting the autonomous menu
    # For now, it's a placeholder
    pass

def user_control():
    """This is the user control function. It is called when the driver control period starts."""
    # Exits the autonomous menu
    exit_auton()
    
    # This loop runs forever, controlling the robot during the driver control period
    while True:
        if not chassis.joystick_touched:
            if (controller1.axis1.position() != 0 or controller1.axis2.position() != 0 or 
                controller1.axis3.position() != 0 or controller1.axis4.position() != 0):
                chassis.joystick_touched = True
        
        if DRIVE_MODE == 0:  # Double arcade
            chassis.control_arcade(controller1.axis2.position(), controller1.axis4.position())
        elif DRIVE_MODE == 1:  # Single arcade
            chassis.control_arcade(controller1.axis3.position(), controller1.axis4.position())
        elif DRIVE_MODE == 2:  # Tank drive
            chassis.control_tank(controller1.axis3.position(), controller1.axis2.position())
        elif DRIVE_MODE == 3:  # Mecanum drive
            chassis.control_mecanum(controller1.axis4.position(), controller1.axis3.position(), 
                                  controller1.axis2.position(), controller1.axis1.position(), 
                                  left_motor1, left_motor2, right_motor1, right_motor2)
        
        # This wait prevents the loop from using too much CPU time
        wait(20, MSEC)
