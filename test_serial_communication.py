#!/usr/bin/env python3
"""
Test script for communicating with VEX V5 brain via serial communication.
This script sends commands to the VEX brain and displays responses.
"""

import serial
import time
import sys

def main():
    # Serial port configuration
    # You may need to change the port name based on your system
    # Windows: 'COM3', 'COM4', etc.
    # Mac/Linux: '/dev/ttyUSB0', '/dev/ttyACM0', etc.
    PORT = '/dev/ttyUSB0'  # Change this to match your system
    BAUD_RATE = 115200
    
    try:
        # Open serial connection
        print(f"Connecting to {PORT} at {BAUD_RATE} baud...")
        ser = serial.Serial(PORT, BAUD_RATE, timeout=1)
        time.sleep(2)  # Wait for connection to stabilize
        
        print("Connected! Press Button X on the VEX controller to start listening.")
        print("Available commands:")
        print("  MOVE_FORWARD - Drive forward 12 inches")
        print("  TURN_LEFT - Turn left 90 degrees")
        print("  TURN_RIGHT - Turn right 90 degrees")
        print("  STOP - Stop the robot")
        print("  STATUS - Get robot status")
        print("  quit - Exit the program")
        print()
        
        # Clear any initial data
        ser.reset_input_buffer()
        ser.reset_output_buffer()
        
        while True:
            # Check for incoming data from VEX brain
            if ser.in_waiting:
                try:
                    response = ser.readline().decode('utf-8').strip()
                    if response:
                        print(f"VEX Response: {response}")
                except UnicodeDecodeError:
                    pass  # Ignore invalid characters
            
            # Get user input
            try:
                command = input("Enter command: ").strip()
                
                if command.lower() == 'quit':
                    break
                elif command:
                    # Send command to VEX brain
                    ser.write(f"{command}\n".encode('utf-8'))
                    ser.flush()
                    print(f"Sent: {command}")
                
            except KeyboardInterrupt:
                print("\nExiting...")
                break
            except EOFError:
                break
    
    except serial.SerialException as e:
        print(f"Serial connection error: {e}")
        print("\nTroubleshooting:")
        print("1. Make sure the VEX brain is connected via USB")
        print("2. Check if the port name is correct")
        print("3. Try different port names:")
        print("   - Windows: COM1, COM2, COM3, etc.")
        print("   - Mac/Linux: /dev/ttyUSB0, /dev/ttyACM0, etc.")
        print("4. Make sure no other program is using the port")
        return 1
    
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("Serial connection closed.")
    
    return 0

if __name__ == "__main__":
    sys.exit(main()) 