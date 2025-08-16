#!/usr/bin/env python3
"""
Drive Class - Python Version (Simplified)
Handles drivetrain control, PID, and movement functions
"""

from vex import *
import math
from .PID import PID
from vex import wait, MSEC, HOLD, FORWARD, VoltageUnits, DEGREES, DEGREES

def normalize_360(angle):
    """Normalize angle to 0-360 degrees"""
    return (angle + 360) % 360

def normalize_180(angle):
    """Normalize angle to -180 to 180 degrees"""
    return ((angle + 540) % 360) - 180

def threshold(input_val, min_val, max_val):
    """Clamp input value between min and max"""
    if input_val > max_val:
        return max_val
    if input_val < min_val:
        return min_val
    return input_val

def to_volt(percent):
    """Convert percentage to voltage (0-100% to 0-12V)"""
    return percent * 12.0 / 100.0

def deadband(input_val, width):
    """Apply deadband to input value"""
    if abs(input_val) < width:
        return 0
    return input_val

class Drive:
    """Drive class for controlling the robot's drivetrain"""
    
    def __init__(self, left_drive, right_drive, gyro, wheel_diameter, gear_ratio):
        """Initialize the Drive class"""
        self.wheel_diameter = wheel_diameter
        self.gear_ratio = gear_ratio
        self.drive_in_to_deg_ratio = gear_ratio / 360.0 * math.pi * wheel_diameter
        self.left_drive = left_drive
        self.right_drive = right_drive
        self.gyro = gyro
        
        # PID constants
        self.turn_max_voltage = 6.0
        self.turn_kp = 1.0
        self.turn_ki = 0.0
        self.turn_kd = 0.0
        self.turn_start_i = 0.0
        
        self.drive_max_voltage = 6.0
        self.drive_kp = 1.0
        self.drive_ki = 0.0
        self.drive_kd = 0.0
        self.drive_start_i = 0.0
        
        self.heading_max_voltage = 6.0
        self.heading_kp = 1.0
        self.heading_kd = 0.0
        
        # Exit conditions
        self.turn_settle_error = 1.0
        self.turn_settle_time = 200.0
        self.turn_timeout = 1500.0
        
        self.drive_settle_error = 1.0
        self.drive_settle_time = 200.0
        self.drive_timeout = 2000.0
        
        # Arcade drive constants
        self.k_brake = 0.5
        self.k_turn_bias = 0.5
        self.k_turn_damping_factor = 0.85
        self.k_throttle = 0.0
        self.k_turn = 0.0
        
        # State variables
        self.desired_heading = 0.0
        self.drivetrain_needs_stopped = False
        self.joystick_touched = False
        self.stop_mode = HOLD
    
    def set_turn_pid(self, turn_max_voltage, turn_kp, turn_ki, turn_kd, turn_start_i):
        """Set turn PID constants"""
        self.turn_max_voltage = turn_max_voltage
        self.turn_kp = turn_kp
        self.turn_ki = turn_ki
        self.turn_kd = turn_kd
        self.turn_start_i = turn_start_i
    
    def set_drive_pid(self, drive_max_voltage, drive_kp, drive_ki, drive_kd, drive_start_i):
        """Set drive PID constants"""
        self.drive_max_voltage = drive_max_voltage
        self.drive_kp = drive_kp
        self.drive_ki = drive_ki
        self.drive_kd = drive_kd
        self.drive_start_i = drive_start_i
    
    def set_heading_pid(self, heading_max_voltage, heading_kp, heading_kd):
        """Set heading PID constants"""
        self.heading_max_voltage = heading_max_voltage
        self.heading_kp = heading_kp
        self.heading_kd = heading_kd
    
    def set_turn_exit_conditions(self, turn_settle_error, turn_settle_time, turn_timeout):
        """Set turn exit conditions"""
        self.turn_settle_error = turn_settle_error
        self.turn_settle_time = turn_settle_time
        self.turn_timeout = turn_timeout
    
    def set_drive_exit_conditions(self, drive_settle_error, drive_settle_time, drive_timeout):
        """Set drive exit conditions"""
        self.drive_settle_error = drive_settle_error
        self.drive_settle_time = drive_settle_time
        self.drive_timeout = drive_timeout
    
    def set_heading(self, orientation_deg):
        """Set the current heading"""
        self.gyro.set_heading(orientation_deg, DEGREES)
        self.desired_heading = orientation_deg
    
    def get_heading(self):
        """Get the current heading"""
        return self.gyro.heading(DEGREES)
    
    def get_left_position_in(self):
        """Get left side position in inches"""
        return self.left_drive.position(DEGREES) * self.drive_in_to_deg_ratio
    
    def get_right_position_in(self):
        """Get right side position in inches"""
        return self.right_drive.position(DEGREES) * self.drive_in_to_deg_ratio
    
    def drive_with_voltage(self, left_voltage, right_voltage):
        """Drive with specified left and right voltages"""
        self.left_drive.spin(FORWARD, left_voltage, VoltageUnits.VOLT)
        self.right_drive.spin(FORWARD, right_voltage, VoltageUnits.VOLT)
    
    def turn_to_heading(self, heading, turn_max_voltage=None):
        """Turn to specified heading"""
        if turn_max_voltage is None:
            turn_max_voltage = self.turn_max_voltage
        
        self.desired_heading = normalize_360(heading)
        
        # Create PID controller for turning
        turn_pid = PID(
            normalize_180(heading - self.get_heading()),
            self.turn_kp, self.turn_ki, self.turn_kd, self.turn_start_i,
            self.turn_settle_error, self.turn_settle_time, self.turn_timeout
        )
        
        while not turn_pid.is_done() and not self.drivetrain_needs_stopped:
            error = normalize_180(heading - self.get_heading())
            output = turn_pid.compute(error)
            output = threshold(output, -turn_max_voltage, turn_max_voltage)
            
            self.drive_with_voltage(output, -output)
            wait(10, MSEC)
        
        if self.drivetrain_needs_stopped:
            self.left_drive.stop(HOLD)
            self.right_drive.stop(HOLD)
    
    def drive_distance(self, distance, drive_max_voltage=None, heading=None, 
                      heading_max_voltage=None, chaining=False, 
                      drive_settle_error=None, drive_settle_time=None):
        """Drive a specified distance"""
        if drive_max_voltage is None:
            drive_max_voltage = self.drive_max_voltage
        if heading is None:
            heading = self.desired_heading
        if heading_max_voltage is None:
            heading_max_voltage = self.heading_max_voltage
        if drive_settle_error is None:
            drive_settle_error = self.drive_settle_error
        if drive_settle_time is None:
            drive_settle_time = self.drive_settle_time
        
        self.desired_heading = normalize_360(heading)
        
        # Create PID controllers
        drive_pid = PID(
            distance, self.drive_kp, self.drive_ki, self.drive_kd, self.drive_start_i,
            drive_settle_error, drive_settle_time, self.drive_timeout
        )
        heading_pid = PID(
            normalize_180(self.desired_heading - self.get_heading()),
            self.heading_kp, 0, self.heading_kd
        )
        
        start_average_position = (self.get_left_position_in() + self.get_right_position_in()) / 2.0
        
        while not drive_pid.is_done() and not self.drivetrain_needs_stopped:
            average_position = (self.get_left_position_in() + self.get_right_position_in()) / 2.0
            drive_error = distance + start_average_position - average_position
            heading_error = normalize_180(self.desired_heading - self.get_heading())
            
            drive_output = drive_pid.compute(drive_error)
            heading_output = heading_pid.compute(heading_error)
            
            drive_output = threshold(drive_output, -drive_max_voltage, drive_max_voltage)
            heading_output = threshold(heading_output, -heading_max_voltage, heading_max_voltage)
            
            self.drive_with_voltage(drive_output + heading_output, drive_output - heading_output)
            wait(10, MSEC)
        
        if self.drivetrain_needs_stopped:
            self.left_drive.stop(HOLD)
            self.right_drive.stop(HOLD)
    
    def set_arcade_constants(self, k_brake, k_turn_bias, k_turn_damping_factor):
        """Set arcade drive constants"""
        self.k_brake = k_brake
        self.k_turn_bias = k_turn_bias
        self.k_turn_damping_factor = k_turn_damping_factor
    
    def control_arcade(self, y, x):
        """Control arcade drive with joystick inputs"""
        throttle = deadband(y, 5)
        turn = deadband(x, 5) * self.k_turn_damping_factor
        
        # Apply curve functions
        turn = self.curve_function(turn, self.k_turn)
        throttle = self.curve_function(throttle, self.k_throttle)
        
        left_power = to_volt(throttle + turn)
        right_power = to_volt(throttle - turn)
        
        # Apply turn bias if enabled
        if self.k_turn_bias > 0:
            if abs(throttle) + abs(turn) > 100:
                old_throttle = throttle
                old_turn = turn
                throttle *= (1 - self.k_turn_bias * abs(old_turn / 100.0))
                turn *= (1 - (1 - self.k_turn_bias) * abs(old_throttle / 100.0))
            
            left_power = to_volt(throttle + turn)
            right_power = to_volt(throttle - turn)
        
        if abs(throttle) > 0 or abs(turn) > 0:
            self.left_drive.spin(FORWARD, left_power, VoltageUnits.VOLT)
            self.right_drive.spin(FORWARD, right_power, VoltageUnits.VOLT)
            self.drivetrain_needs_stopped = True
        else:
            # When joysticks are released, run active brake on drive
            if self.drivetrain_needs_stopped:
                if self.stop_mode != HOLD:
                    self.left_drive.reset_position()
                    self.right_drive.reset_position()
                    wait(20, MSEC)
                    self.left_drive.spin(FORWARD, -self.left_drive.position(DEGREES) * self.k_brake, VoltageUnits.VOLT)
                    self.right_drive.spin(FORWARD, -self.right_drive.position(DEGREES) * self.k_brake, VoltageUnits.VOLT)
                else:
                    self.left_drive.stop(HOLD)
                    self.right_drive.stop(HOLD)
                self.drivetrain_needs_stopped = False
    
    def control_tank(self, left, right):
        """Control tank drive with left and right joystick inputs"""
        left_throttle = self.curve_function(left, self.k_throttle)
        right_throttle = self.curve_function(right, self.k_throttle)
        
        if abs(left_throttle) > 0 or abs(right_throttle) > 0:
            self.left_drive.spin(FORWARD, to_volt(left_throttle), VoltageUnits.VOLT)
            self.right_drive.spin(FORWARD, to_volt(right_throttle), VoltageUnits.VOLT)
            self.drivetrain_needs_stopped = True
        else:
            if self.drivetrain_needs_stopped:
                self.left_drive.stop(self.stop_mode)
                self.right_drive.stop(self.stop_mode)
                self.drivetrain_needs_stopped = False
    
    def control_mecanum(self, x, y, acc, steer, drive_lf, drive_rf, drive_lb, drive_rb):
        """Control mecanum drive with four joystick inputs"""
        throttle = deadband(y, 5)
        strafe = deadband(x, 5)
        straight = deadband(acc, 5)
        turn = deadband(steer, 5)
        
        straight = self.curve_function(straight, self.k_throttle)
        turn = self.curve_function(turn, self.k_turn)
        
        if turn == 0 and strafe == 0 and throttle == 0 and straight == 0:
            if self.drivetrain_needs_stopped:
                self.left_drive.stop(self.stop_mode)
                self.right_drive.stop(self.stop_mode)
                self.drivetrain_needs_stopped = False
                return
        
        if turn == 0 and straight == 0:
            # Strafe mode
            drive_lf.spin(FORWARD, to_volt(throttle + turn + strafe), VoltageUnits.VOLT)
            drive_rf.spin(FORWARD, to_volt(throttle - turn - strafe), VoltageUnits.VOLT)
            drive_lb.spin(FORWARD, to_volt(throttle + turn - strafe), VoltageUnits.VOLT)
            drive_rb.spin(FORWARD, to_volt(throttle - turn + strafe), VoltageUnits.VOLT)
        else:
            # Arcade drive mode
            left_power = to_volt(throttle + turn)
            right_power = to_volt(throttle - turn)
            self.left_drive.spin(FORWARD, left_power, VoltageUnits.VOLT)
            self.right_drive.spin(FORWARD, right_power, VoltageUnits.VOLT)
            self.drivetrain_needs_stopped = True
    
    def stop(self, mode):
        """Stop the drivetrain with specified brake mode"""
        self.drivetrain_needs_stopped = True
        self.left_drive.stop(mode)
        self.right_drive.stop(mode)
        self.stop_mode = mode
        self.left_drive.reset_position()
        self.right_drive.reset_position()
        self.drivetrain_needs_stopped = False
    
    def check_status(self):
        """Check and display robot status"""
        distance_traveled = (self.get_left_position_in() + self.get_right_position_in()) / 2.0
        heading = self.get_heading()
        
        # Display heading and distance traveled on the controller screen
        status_msg = f"heading: {int(heading)}, dist: {int(distance_traveled)}"
        print(status_msg)
    
    def curve_function(self, x, curve_scale):
        """Apply curve function to input for smoother control"""
        if curve_scale == 0:
            return x
        
        return (math.exp(-(curve_scale / 10)) + 
                math.exp((abs(x) - 100) / 10) * (1 - math.exp(-(curve_scale / 10)))) * x
