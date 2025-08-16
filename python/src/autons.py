#!/usr/bin/env python3

from vex import *
from .robot_config import chassis, brain
from vex import wait, MSEC, BRAKE

# Global variables
current_auton_selection = 0        # Current auton selection
auton_test_step = 0                # Current step in auton

def sample_auton1():
    """The first autonomous routine"""
    chassis.drive_with_voltage(3, 3)
    wait(1000, MSEC)
    chassis.stop(BRAKE)

def sample_auton2():
    """The second autonomous routine"""
    chassis.set_heading(180)  # Place the robot backwards
    chassis.drive_distance(-12, 6)
    chassis.turn_to_heading(0, 6)
    chassis.drive_distance(12, 6)

def sample_skill():
    """A long autonomous routine, e.g. skill.
    This routine is broken into steps to allow for testing of individual steps.
    This allows for easier debugging of individual parts of the long autonomous routine."""
    global auton_test_step
    
    if auton_test_step == 0:
        chassis.turn_to_heading(180)
        if not continue_auton_step():
            return  # If in test mode, stop here for testing.
    
    if auton_test_step == 1:
        chassis.drive_distance(5)
        chassis.turn_to_heading(chassis.get_heading() + 90)  # Turn right
        if not continue_auton_step():
            return  # If in test mode, stop here for testing.
    
    if auton_test_step == 2:
        chassis.turn_to_heading(chassis.get_heading() - 90)  # Turn left
        chassis.drive_distance(-5)

def run_auton_item():
    """Runs the selected autonomous routine"""
    if current_auton_selection == 0:
        sample_auton1()
    elif current_auton_selection == 1:
        sample_auton2()
    elif current_auton_selection == 2:
        sample_skill()

# The names of the autonomous routines to be displayed in the menu
auton_menu_text = [
    "auton1",
    "auton2",
    "auton_skill"
]

# ----------------------------------------------------------------------------
#                 Only change code below this line when necessary
# ----------------------------------------------------------------------------

# Total number of autons, automatically calculated based on the size of the autonMenuText array
auton_num = len(auton_menu_text)
auton_test_mode = False           # Indicates if in test mode
exit_auton_menu = False           # Flag to exit the autonomous menu
enable_end_game_timer = False     # Flag to indicate if endgame timer is enabled
END_GAME_SECONDS = 85            # Endgame reminder starts at 85 seconds

def autonomous():
    """The function is called when the autonomous period starts"""
    global exit_auton_menu, enable_end_game_timer
    
    # Exits the autonomous menu
    exit_auton_menu = True
    enable_end_game_timer = True
    
    # Runs the selected autonomous routine
    run_auton_item()

def print_menu_item():
    """This function prints the selected autonomous routine to the brain and controller screens"""
    if current_auton_selection < 0:
        print_controller_screen("quick test")
        return
    
    # Clears the brain screen
    brain.screen.clear_screen()
    
    # Sets the cursor to the third row, first column
    brain.screen.set_cursor(3, 1)
    
    # Prints the selected autonomous routine name
    brain.screen.print(auton_menu_text[current_auton_selection])
    print_controller_screen(auton_menu_text[current_auton_selection])

def continue_auton_step():
    """Continue to the next autonomous step if in test mode"""
    global auton_test_step
    
    if auton_test_mode:
        auton_test_step += 1
        return False  # Stop execution for testing
    return True  # Continue execution

def exit_auton():
    """Exit the autonomous menu"""
    # This function would handle exiting the autonomous menu
    pass

def print_controller_screen(message):
    """Print a message to the controller screen"""
    # This function would print to the controller screen
    # For now, just print to console
    print(f"Controller: {message}")

def register_auton_test_buttons():
    """Register autonomous testing buttons"""
    # This function would register buttons for testing autonomous routines
    # For now, it's a placeholder
    pass

def initialize_chassis():
    """Initialize the chassis with default settings"""
    # This function would initialize the chassis with default PID and other settings
    # For now, it's a placeholder
    pass
