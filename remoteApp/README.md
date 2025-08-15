# VEX Robot Control iOS App

A native iOS application for controlling VEX robots using speech recognition and gyroscope input.

## Features

### 🎤 Speech Control
- **Voice Commands**: Speak commands that are sent directly to the robot via WebSocket
- **Real-time Recognition**: See recognized text as you speak
- **Automatic Transmission**: Commands are automatically sent when speech recognition is complete

### 📱 Gyroscope Control
- **Tilt to Turn**: Tilt your phone left/right to control robot turning
- **Forward/Backward**: Use buttons to control forward and backward movement
- **Stop Button**: Emergency stop functionality
- **Voltage Control**: Precise voltage control with sensitivity limits (±0.2V for turning, ±0.3V for forward/backward)

### 🔌 WebSocket Communication
- **Device ID Connection**: Connect to your robot using its device ID
- **IP Address Input**: Manually enter the robot's IP address (unlike web app which auto-detects)
- **Real-time Control**: Low-latency communication with the robot
- **Automatic Reconnection**: Handles connection drops gracefully

## Setup Instructions

### 1. Prerequisites
- iOS device with microphone and motion sensors
- VEX robot running the WebSocket server
- Device ID from your robot

### 2. Permissions
The app will request the following permissions:
- **Microphone Access**: Required for speech recognition
- **Speech Recognition**: Required for converting voice to text
- **Motion Data**: Automatically granted for gyroscope access

### 3. Connection
1. Launch the app
2. Enter your robot's IP address (e.g., 192.168.1.100 or localhost)
3. Enter your robot's device ID
4. Tap "Connect"
5. Wait for connection confirmation

## Usage

### Speech Control
1. Tap "Start Listening" to begin speech recognition
2. Speak your command clearly
3. The recognized text will appear on screen
4. The command is automatically sent to the robot
5. Tap "Stop Listening" when done

### Gyroscope Control
1. **Forward/Backward**: Use the Forward/Backward buttons
2. **Turning**: Tilt your phone left or right
   - Left tilt = Turn left
   - Right tilt = Turn right
3. **Stop**: Use the Stop button to halt all movement

### Control Sensitivity
- **Forward/Backward**: ±0.3V (moderate speed)
- **Turning**: ±0.2V (smooth turning)
- **Maximum Voltage**: ±0.5V (safety limit)

## Technical Details

### WebSocket Protocol
- **Connection**: `ws://{ipAddress}:7071/vexrobotics.vexcode/device?id={deviceId}`
- **Drive Command**: `vol {leftVoltage} {rightVoltage}`
- **Speech Command**: Raw recognized text

### Motion Data
- **Update Rate**: 10 Hz
- **Threshold**: 0.1 rad/s (prevents drift)
- **Axis**: Z-axis rotation for turning

### Speech Recognition
- **Language**: English (US)
- **Mode**: Continuous with partial results
- **Audio Format**: 1024 buffer size, measurement mode

## Troubleshooting

### Connection Issues
- Verify device ID is correct
- Check robot WebSocket server is running
- Ensure network connectivity

### Speech Recognition Issues
- Grant microphone permissions
- Speak clearly and in a quiet environment
- Check device language settings

### Motion Control Issues
- Ensure device has motion sensors
- Calibrate device motion sensors
- Check for magnetic interference

## Requirements

- iOS 14.0+
- iPhone/iPad with microphone and motion sensors
- Network access to robot WebSocket server

## Architecture

The app uses a clean MVVM architecture:
- **ContentView**: Main UI and user interaction
- **RobotController**: Business logic for WebSocket, speech, and motion
- **ObservableObject**: Reactive data binding with SwiftUI

## License

This project is part of the VEX Robotics control system.
