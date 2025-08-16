# VEX Robot Control iOS App - Complete Developer Setup Guide

## 🎯 Project Overview

This document provides complete technical details to recreate a native iOS app that controls VEX robots using:
- **Speech Recognition**: Voice commands sent directly to the robot
- **4-Way Tilt Control**: Phone orientation for precise robot movement
- **WebSocket Communication**: Real-time connection to VEX robotics servers
- **Additive Voltage Mapping**: Intuitive control combining forward/backward and left/right movements

## 📱 App Architecture

### Two-Screen Design
1. **Connection Screen** (Portrait): IP address and device ID input
2. **Drive Screen** (Portrait): Robot control with speech and tilt

### Core Components
- `ContentView.swift`: Main app coordinator and screen switching
- `RobotController.swift`: WebSocket, speech recognition, and motion data
- `ConnectionScreen.swift`: Connection UI (embedded in ContentView)
- `DriveScreen.swift`: Robot control UI (embedded in ContentView)

## 🛠️ Xcode Project Setup

### 1. Create New Project
1. Open Xcode → Create a new Xcode project
2. Choose **iOS** → **App**
3. Product Name: `remoteApp`
4. Interface: **SwiftUI**
5. Language: **Swift**

### 2. Required Frameworks
Add these frameworks to your target's "Build Phases" → "Link Binary With Libraries":

```
+ Speech.framework
+ CoreMotion.framework  
+ Network.framework
+ AVFoundation.framework
```

### 3. Info.plist Configuration
**CRITICAL**: Add these keys to Info.plist for permissions and orientation:

```xml
<!-- Microphone Permission -->
<key>NSMicrophoneUsageDescription</key>
<string>This app needs microphone access for speech recognition to control the robot.</string>

<!-- Speech Recognition Permission -->
<key>NSSpeechRecognitionUsageDescription</key>
<string>This app uses speech recognition to convert voice commands into robot control instructions.</string>

<!-- Interface Orientations (CRITICAL for dynamic switching) -->
<key>UISupportedInterfaceOrientations</key>
<array>
    <string>UIInterfaceOrientationPortrait</string>
    <string>UIInterfaceOrientationLandscapeLeft</string>
    <string>UIInterfaceOrientationLandscapeRight</string>
</array>

<key>UISupportedInterfaceOrientations~ipad</key>
<array>
    <string>UIInterfaceOrientationPortrait</string>
    <string>UIInterfaceOrientationLandscapeLeft</string>
    <string>UIInterfaceOrientationLandscapeRight</string>
</array>
```

### 4. App Transport Security (for local testing)
Add to Info.plist for WebSocket testing:

```xml
<key>NSAppTransportSecurity</key>
<dict>
    <key>NSAllowsArbitraryLoads</key>
    <true/>
</dict>
```

## 📱 Device Setup

### 1. iPhone Requirements
- **iOS Version**: 14.0+
- **Hardware**: Must have motion sensors (gyroscope, accelerometer)
- **Network**: Local network access to robot

### 2. Developer Certificate
1. **Development Team**: Select your team in project settings
2. **Bundle Identifier**: Unique identifier (e.g., `com.yourname.remoteApp`)
3. **Signing**: Automatic or manual signing

### 3. Device Trust
After installing on iPhone:
1. Go to **Settings** → **General** → **VPN & Device Management**
2. Tap your developer certificate
3. Tap **Trust** to allow the app to run

## 🔧 Core Implementation

### 1. RobotController.swift
**Purpose**: Handles WebSocket communication, speech recognition, and motion data

**Key Features**:
- WebSocket connection to VEX robotics servers
- Speech recognition using Speech framework
- Device motion tracking using CoreMotion
- Permission management

**Critical Methods**:
```swift
func connect(deviceId: String, ipAddress: String)
func startSpeechRecognition()
func stopSpeechRecognition()
private func setupMotionManager()
```

### 2. ContentView.swift
**Purpose**: Main app coordinator, handles screen switching and orientation

**Key Features**:
- Conditional view switching between Connection and Drive screens
- Dynamic orientation control (portrait ↔ landscape)
- Global state management

**Screen Switching Logic**:
```swift
Group {
    if !isConnected {
        ConnectionScreen(...)
    } else {
        DriveScreen(...)
    }
}
```

### 3. ConnectionScreen.swift
**Purpose**: Connection UI for IP address and device ID input

**Key Features**:
- TextField for IP address (decimalPad keyboard)
- TextField for device ID (numberPad keyboard)
- Connect button with validation
- Keyboard dismissal with "Done" button

### 4. DriveScreen.swift
**Purpose**: Robot control interface with speech and tilt controls

**Key Features**:
- Speech control button (left side)
- Drive/Stop button (right side)
- 4-way tilt control using device motion
- Command display at bottom

## 🎮 Tilt Control System

### 1. Motion Data Source
```swift
// Device motion provides absolute angles
motionManager.startDeviceMotionUpdates { motion, error in
    let pitchDegrees = motion.attitude.pitch * 180.0 / .pi  // Forward/backward
    let rollDegrees = motion.attitude.roll * 180.0 / .pi    // Left/right
}
```

### 2. Angle-to-Voltage Mapping
**Dead Zone**: 0-8° = 0V (no movement)
**Linear Range**: 8° to 30° = 0V to ±6V
**Maximum**: >30° = ±6V (full speed)

**Formula**:
```swift
let normalizedTilt = (abs(tiltAngle) - 8.0) / 22.0  // 0.0 to 1.0
let voltage = normalizedTilt * 6.0  // 0V to 6V
```

### 3. Additive Voltage System
**Base**: 0V when phone is flat
**Forward/Backward**: Adds/subtracts equally to both motors
**Left/Right**: Adds to left motor, subtracts from right motor

**Voltage Calculation**:
```swift
leftVoltage = pitchVoltage + rollVoltage   // Forward + Left
rightVoltage = pitchVoltage - rollVoltage  // Forward - Left
```

**Example Movements**:
- **Forward 30° + Left 30°**: Left=12V, Right=0V
- **Just Left 30°**: Left=6V, Right=-6V
- **Forward 30°**: Left=6V, Right=6V

## 🌐 WebSocket Communication

### 1. Connection URL Format
**For Testing**:
```
ws://localhost:7071
```

**For Real VEX Robotics**:
```
ws://{IP_ADDRESS}/vexrobotics.vexcode/device?id={DEVICE_ID}
```

### 2. Command Protocol
**Drive Command**: `vol {leftVoltage} {rightVoltage}`
**Speech Command**: `{recognized_text}`

### 3. Connection Management
- Automatic reconnection handling
- Error logging and status updates
- Message queuing and delivery confirmation

## 🎤 Speech Recognition System

### 1. Setup Process
```swift
// Request authorization
SFSpeechRecognizer.requestAuthorization { authStatus in
    // Handle authorization result
}

// Configure audio session
let audioSession = AVAudioSession.sharedInstance()
try audioSession.setCategory(.record, mode: .measurement)
try audioSession.setActive(true)
```

### 2. Recognition Flow
1. User taps "Start Listening"
2. Audio engine starts recording
3. Speech recognition processes audio
4. Recognized text displayed in real-time
5. Final text sent to robot via WebSocket
6. User taps "Stop Listening"

### 3. Audio Configuration
- **Sample Rate**: Hardware default
- **Buffer Size**: 1024 samples
- **Format**: Input node's output format

## 📱 UI Implementation Details

### 1. Button States
**Speech Button**:
- Default: Blue with microphone icon
- Listening: Red with filled microphone icon
- Text: "Start Listening" ↔ "Stop Listening"

**Drive Button**:
- Default: Green with play icon
- Driving: Red with stop icon
- Text: "Drive" ↔ "Stop"

### 2. Layout Structure
**Connection Screen**:
- Header with robot icon and title
- IP address input field
- Device ID input field
- Connect button
- Centered vertically with proper spacing

**Drive Screen**:
- Speech control (top, 200x120)
- Drive control (bottom, 200x120)
- Command display (bottom)
- Both buttons same size, vertically centered

### 3. Orientation Handling
**Dynamic Switching**:
- Connection screen: Portrait only
- Drive screen: Portrait only
- Automatic switching based on connection state

## 🔍 Debugging and Testing

### 1. Console Output
**Connection Debug**:
```
RobotController: Attempting to connect to: ws://192.168.86.22/vexrobotics.vexcode/device?id=123
RobotController: URL created successfully: ws://192.168.86.22/vexrobotics.vexcode/device?id=123
RobotController: WebSocket connection initiated, isConnected set to true
```

**Motion Debug**:
```
RobotController: Device pitch: 25.3°, Roll: -12.1°
Pitch: 25.3°, voltage: 4.7V, stable count: 3
Combined: Pitch=4.7V, Roll=0.0V → Left=4.7V, Right=4.7V
```

### 2. Common Issues
**Build Errors**:
- Missing frameworks: Add to Build Phases
- Permission errors: Check Info.plist
- Orientation issues: Verify UISupportedInterfaceOrientations

**Runtime Issues**:
- Microphone not working: Check NSMicrophoneUsageDescription
- Motion not responding: Verify device has sensors
- WebSocket connection failed: Check network and URL format

## 🚀 Build and Deploy

### 1. Build Process
1. Select target device (iPhone/iPad)
2. Press Cmd+R to build and run
3. Grant permissions when prompted
4. Test all functionality

### 2. Device Installation
1. Connect iPhone to Mac
2. Select device in Xcode
3. Build and run
4. Trust developer certificate on iPhone

### 3. Testing Checklist
- [ ] App launches without errors
- [ ] Permissions requested and granted
- [ ] Connection screen displays correctly
- [ ] Can connect to WebSocket server
- [ ] Speech recognition works
- [ ] Tilt control responds to phone movement
- [ ] Commands sent to robot successfully

## 📚 Key Technical Concepts

### 1. SwiftUI State Management
- `@StateObject` for RobotController
- `@State` for local view state
- `@ObservedObject` for data binding
- `onReceive` for publisher subscriptions

### 2. CoreMotion Integration
- Device motion vs. gyroscope data
- Absolute angles vs. relative movement
- Smoothing and filtering algorithms
- Battery-efficient update intervals

### 3. WebSocket Implementation
- URLSession WebSocket tasks
- Message handling and error recovery
- Connection state management
- Real-time bidirectional communication

### 4. Speech Recognition
- Authorization and permissions
- Audio session configuration
- Real-time vs. final results
- Error handling and fallbacks

## 🔒 Security and Permissions

### 1. Required Permissions
- **Microphone**: For speech recognition
- **Speech Recognition**: For voice-to-text conversion
- **Motion Data**: Automatically granted (no permission dialog)

### 2. Network Security
- **Local Network**: WebSocket over HTTP (no encryption)
- **Production**: Should use WSS (WebSocket Secure) with TLS
- **Device Authentication**: Via device ID parameter

### 3. Data Privacy
- No persistent storage of voice data
- Motion data processed locally only
- WebSocket messages not logged permanently

## 🎯 Performance Optimization

### 1. Motion Updates
- **Update Rate**: 10 Hz (100ms intervals)
- **Processing**: Main thread for UI updates
- **Smoothing**: Exponential moving average

### 2. Speech Recognition
- **Partial Results**: Enabled for real-time feedback
- **Audio Quality**: Measurement mode for accuracy
- **Memory Management**: Proper cleanup of audio resources

### 3. WebSocket
- **Connection Pooling**: Single connection per session
- **Message Batching**: Commands sent immediately
- **Error Recovery**: Automatic reconnection attempts

## 🚧 Troubleshooting Guide

### 1. Build Issues
**"Multiple commands produce Info.plist"**
- Delete manually created Info.plist
- Let Xcode manage it automatically

**"Type has no member" errors**
- Check framework linking
- Verify import statements
- Clean build folder (Shift+Cmd+K)

### 2. Runtime Issues
**App crashes on launch**
- Check Info.plist permissions
- Verify framework linking
- Check device compatibility

**Speech recognition not working**
- Verify microphone permission
- Check audio session configuration
- Test on physical device (not simulator)

**Motion control not responding**
- Verify device has required sensors
- Check CoreMotion framework
- Test on physical device

**WebSocket connection failed**
- Check network connectivity
- Verify URL format
- Check server status
- Review ATS settings for local connections

## 📱 Testing on Simulator vs Device

### 1. Simulator Limitations
- **Motion Sensors**: Limited simulation
- **Microphone**: May not work properly
- **WebSocket**: Works for network testing

### 2. Device Testing
- **Motion Sensors**: Full functionality
- **Microphone**: Real speech recognition
- **WebSocket**: Actual network conditions
- **Performance**: Real-world performance data

## 🎉 Conclusion

This guide provides everything needed to recreate the VEX robot control iOS app. The key to success is:

1. **Proper Xcode setup** with all required frameworks
2. **Correct Info.plist configuration** for permissions and orientation
3. **Understanding the additive voltage mapping** system
4. **Testing on physical devices** for motion and speech features
5. **Proper WebSocket URL formatting** for VEX robotics servers

The app demonstrates modern iOS development practices with SwiftUI, real-time sensor data processing, and network communication, providing an intuitive interface for robot control.
