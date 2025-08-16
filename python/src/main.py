#!/usr/bin/env python3
"""
VEX Robot Control - Python Version (Simplified)
Main control loop and command processing
"""

from vex import *
import time

from .robot_config import chassis, in_take, out_take, stop_rollers, left_motor1, left_motor2, right_motor1, right_motor2
from vex import wait, MSEC, HOLD, COAST

# Global instances
brain = Brain()
controller1 = Controller(PRIMARY)

# Global variables
# Note: serial_port was removed since we're not using serial communication
# Add any other global variables you need here

def sample_button_l1_action():
    """Function called when L1 button is pressed"""
    in_take()
    
    # Wait until button is released to stop the rollers
    try:
        while controller1.buttonL1.pressing():
            wait(20, MSEC)
    except AttributeError:
        try:
            while controller1.buttonL1.is_pressed():
                wait(20, MSEC)
        except AttributeError:
            print("Button pressing check not supported")
    
    stop_rollers()

def button_l2_action():
    """Function called when L2 button is pressed"""
    out_take()
    
    # Wait until button is released to stop the rollers
    try:
        while controller1.buttonL2.pressing():
            wait(20, MSEC)
    except AttributeError:
        try:
            while controller1.buttonL2.is_pressed():
                wait(20, MSEC)
        except AttributeError:
            print("Button pressing check not supported")
    
    stop_rollers()

def button_r2_action():
    """Function called when R2 button is pressed"""
    # Brake the drivetrain until button is released
    chassis.stop(HOLD)
    try:
        controller1.rumble(".")
    except AttributeError:
        print("Controller rumble not supported")
    
    try:
        while controller1.buttonR2.pressing():
            wait(10, MSEC)
    except AttributeError:
        try:
            while controller1.buttonR2.is_pressed():
                wait(10, MSEC)
        except AttributeError:
            print("Button pressing check not supported")
    
    chassis.check_status()
    chassis.stop(COAST)

def setup_button_mapping():
    """Set up button mapping for the controller"""
    # Note: VEX Python API may use different method names
    try:
        # Try Python API method names
        controller1.buttonL1.pressed = sample_button_l1_action
        controller1.buttonL2.pressed = button_l2_action
        controller1.buttonR2.pressed = button_r2_action
    except AttributeError:
        try:
            # Try alternative method names
            controller1.buttonL1.on_press(sample_button_l1_action)
            controller1.buttonL2.on_press(button_l2_action)
            controller1.buttonR2.on_press(button_r2_action)
        except AttributeError:
            print("Button mapping not supported in this VEX Python version")
            print("Available button methods:", [m for m in dir(controller1.buttonL1) if 'press' in m.lower()])



def autonomous():
    """Autonomous function - called during autonomous period"""
    # This function runs during the autonomous period
    # You can add autonomous routines here like:
    # - Drive forward
    # - Turn to specific angles
    # - Run intake/outtake sequences
    # - Follow pre-programmed paths
    
    # For now, it's a placeholder
    # You can import and call functions from autons.py here
    pass

def exit_auton():
    """Exit the autonomous menu"""
    # This function handles exiting the autonomous menu
    # For now, it's a placeholder
    pass

def user_control():
    """User control function - called during driver control period"""
    # Exits the autonomous menu
    exit_auton()
    
    # This loop runs forever, controlling the robot during the driver control period
    while True:
        if not chassis.joystick_touched:
            # Note: VEX Python API may use different method names
            try:
                # Try Python API method names
                if (controller1.axis1.position != 0 or controller1.axis2.position != 0 or 
                    controller1.axis3.position != 0 or controller1.axis4.position != 0):
                    chassis.joystick_touched = True
            except AttributeError:
                try:
                    # Try alternative method names
                    if (controller1.axis1.value != 0 or controller1.axis2.value != 0 or 
                        controller1.axis3.value != 0 or controller1.axis4.value != 0):
                        chassis.joystick_touched = True
                except AttributeError:
                    print("Axis position not supported in this VEX Python version")
        
        # Get drive mode from robot_config
        from .robot_config import DRIVE_MODE
        
        if DRIVE_MODE == 0:  # Double arcade
            try:
                chassis.control_arcade(controller1.axis2.position, controller1.axis4.position)
            except AttributeError:
                try:
                    chassis.control_arcade(controller1.axis2.value, controller1.axis4.value)
                except AttributeError:
                    print("Arcade control not supported")
        elif DRIVE_MODE == 1:  # Single arcade
            try:
                chassis.control_arcade(controller1.axis3.position, controller1.axis4.position)
            except AttributeError:
                try:
                    chassis.control_arcade(controller1.axis3.value, controller1.axis4.value)
                except AttributeError:
                    print("Arcade control not supported")
        elif DRIVE_MODE == 2:  # Tank drive
            try:
                chassis.control_tank(controller1.axis3.position, controller1.axis2.position)
            except AttributeError:
                try:
                    chassis.control_tank(controller1.axis3.value, controller1.axis2.value)
                except AttributeError:
                    print("Tank control not supported")
        elif DRIVE_MODE == 3:  # Mecanum drive
            try:
                chassis.control_mecanum(controller1.axis4.position, controller1.axis3.position, 
                                      controller1.axis2.position, controller1.axis1.position, 
                                      left_motor1, left_motor2, right_motor1, right_motor2)
            except AttributeError:
                try:
                    chassis.control_mecanum(controller1.axis4.value, controller1.axis3.value, 
                                          controller1.axis2.value, controller1.axis1.value, 
                                          left_motor1, left_motor2, right_motor1, right_motor2)
                except AttributeError:
                    print("Mecanum control not supported")
        
        # This wait prevents the loop from using too much CPU time
        wait(20, MSEC)

def pre_auton():
    """Pre-autonomous function - called before autonomous period"""
    # This function runs before autonomous period starts
    # You can add setup code here like:
    # - Initialize sensors
    # - Set default PID values
    # - Configure robot settings
    pass

def register_auton_test_buttons():
    """Register autonomous testing buttons"""
    # This function would register buttons for testing autonomous routines
    # For now, it's a placeholder that can be extended later
    pass

def exit_auton():
    """Exit the autonomous menu"""
    # This function handles exiting the autonomous menu
    # For now, it's a placeholder
    pass

def main():
    """Main function"""
# create competition instance

    comp = Competition(user_control, autonomous)
    # Comment out the following line to disable auton testing
    register_auton_test_buttons()
    
    # Set up button mapping for the controller
    setup_button_mapping()
    
    # Run the pre-autonomous function
    pre_auton()
    
    # Prevent main from exiting with an infinite loop
    while True:
        # Note: poll_command_messages() was removed
        # You can add custom communication logic here if needed
        # Examples:
        # - Check for controller button combinations
        # - Process VEX radio messages
        # - Handle brain screen touch input
        
        wait(200, MSEC)

if __name__ == "__main__":
    main()
