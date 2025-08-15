# VEX Robot Control iOS App - Project Summary

## 🎯 Project Overview

This iOS app provides native control of VEX robots using:
- **Speech Recognition**: Voice commands sent directly to the robot
- **Gyroscope Control**: Tilt your phone to control robot turning
- **WebSocket Communication**: Real-time connection to the robot

## 📱 App Features

### Main Interface (ContentView.swift)
- **Landscape Orientation**: Optimized for horizontal device usage
- **Connection Panel**: IP address and device ID input with connection status
- **Speech Control**: Start/stop listening with visual feedback
- **Gyro Control**: Forward/backward buttons + tilt-to-turn
- **Real-time Updates**: Live connection and command status

### Core Controller (RobotController.swift)
- **WebSocket Management**: Handles robot communication
- **Speech Recognition**: Converts voice to text commands
- **Motion Processing**: Processes gyroscope data for turning
- **Permission Management**: Handles microphone and motion access

### Testing Tools
- **WebSocketTest.swift**: Connection testing utility
- **TestView.swift**: Debug interface for testing commands

## 🔧 Technical Implementation

### WebSocket Protocol
```
Connection: ws://{ipAddress}:7071/vexrobotics.vexcode/device?id={deviceId}
Drive Command: vol {leftVoltage} {rightVoltage}
Speech Command: {recognized_text}
```

### Control Sensitivity
- **Forward/Backward**: ±0.3V (moderate speed)
- **Turning**: ±0.2V (smooth control)
- **Maximum**: ±0.5V (safety limit)
- **Threshold**: 0.1 rad/s (prevents drift)

### Motion Data
- **Update Rate**: 10 Hz
- **Axis**: Z-axis rotation for turning
- **Processing**: Real-time voltage calculation

## 📋 Setup Instructions

### 1. Xcode Project Configuration
1. Open project in Xcode
2. Add required frameworks:
   - Speech.framework
   - CoreMotion.framework
   - Network.framework
3. Ensure Info.plist contains permission keys

### 2. Permissions
- **Microphone**: Required for speech recognition
- **Speech Recognition**: Required for voice-to-text
- **Motion Data**: Automatically granted

### 3. Build and Run
1. Select target device (iPhone/iPad)
2. Build and run (Cmd+R)
3. Grant permissions when prompted

## 🚀 Usage Guide

### Connecting to Robot
1. Enter the robot's IP address (e.g., 192.168.1.100)
2. Enter device ID from your VEX robot
3. Tap "Connect"
4. Wait for connection confirmation

### Speech Control
1. Tap "Start Listening"
2. Speak your command clearly
3. Command is automatically sent when complete
4. Tap "Stop Listening" when done

### Gyroscope Control
1. **Forward/Backward**: Use dedicated buttons
2. **Turning**: Tilt phone left/right
3. **Stop**: Emergency stop button
4. **Combined**: Forward + tilt = curved movement

## 🧪 Testing and Debugging

### WebSocket Test
- Use TestView to verify connection
- Test individual commands
- Monitor connection status

### Common Issues
- **Connection Failed**: Check device ID and server status
- **Speech Not Working**: Verify microphone permissions
- **Motion Not Responding**: Check device sensor support

## 📊 Performance Characteristics

### Latency
- **WebSocket**: <10ms typical
- **Speech Recognition**: 500ms-2s depending on command length
- **Motion Response**: 100ms (10Hz update rate)

### Battery Impact
- **Speech Recognition**: Moderate (microphone active)
- **Motion Sensing**: Low (hardware accelerated)
- **WebSocket**: Minimal (efficient protocol)

## 🔒 Security Considerations

### Network
- WebSocket over HTTP (local network)
- No encryption for local development
- Device ID authentication

### Permissions
- Microphone access only when listening
- Motion data processed locally
- No data stored persistently

## 🚧 Future Enhancements

### Potential Features
- **Custom Commands**: User-defined voice commands
- **Gesture Control**: Swipe and tap gestures
- **Haptic Feedback**: Tactile response for actions
- **Command History**: Log of sent commands
- **Multiple Robots**: Support for robot teams

### Technical Improvements
- **WebSocket Security**: TLS encryption
- **Offline Mode**: Command caching
- **Performance**: Lower latency motion control
- **Accessibility**: Voice feedback and larger controls

## 📚 Dependencies

### iOS Frameworks
- **SwiftUI**: Modern UI framework
- **Speech**: Speech recognition
- **CoreMotion**: Motion and orientation data
- **Network**: WebSocket communication

### System Requirements
- **iOS Version**: 14.0+
- **Device**: iPhone/iPad with motion sensors
- **Network**: Local network access to robot

## 🎉 Conclusion

This iOS app provides a comprehensive, user-friendly interface for controlling VEX robots using modern iOS capabilities. The combination of speech recognition and gyroscope control offers intuitive robot operation while maintaining precise control through voltage-based commands.

The app is designed for both development/testing and production use, with built-in debugging tools and a clean, responsive interface optimized for landscape orientation.
