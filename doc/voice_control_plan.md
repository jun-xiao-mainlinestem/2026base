# Voice-Controlled Robot Plan

## Project Overview

This project implements voice-controlled robot commands using the VEX VS Code Extension's WebSocket Server feature. The system allows users to control a VEX V5 robot through voice commands using a web interface, creating an intuitive and hands-free control experience.

### Goals
- Enable voice control of VEX V5 robot movements and mechanisms
- Provide real-time status feedback from robot to web interface
- Create a user-friendly web interface for voice command input
- Implement reliable communication between web interface and robot
- Support continuous voice recognition for seamless operation

### Success Criteria
- Voice commands are recognized with high accuracy
- Robot responds immediately to voice commands
- Status updates are displayed in real-time on web interface
- System operates reliably without manual intervention
- User experience is intuitive and responsive

## System Architecture

### High-Level Architecture
```
Voice Commands → Web Interface → WebSocket → VEX Extension → Serial → VEX Brain → Robot Actions
```

### Component Responsibilities

#### 1. Web Interface
- **Speech Recognition**: Convert voice input to text using Web Speech API
- **Command Processing**: Validate and map voice commands to robot actions
- **User Interface**: Provide intuitive controls and status display
- **WebSocket Client**: Communicate with VEX Extension

#### 2. VEX Extension
- **WebSocket Server**: Receive commands from web interface
- **Serial Bridge**: Forward commands to VEX brain via USB
- **Message Routing**: Handle bidirectional communication

#### 3. VEX Brain
- **RemoteControl Module**: Handle serial communication via USB user port
- **Command Execution**: Execute robot movements and mechanism control
- **Status Reporting**: Send robot state back to web interface
- **Error Handling**: Provide feedback for failed operations

## Implementation Phases

### Phase 1: Foundation Setup
**Duration**: 1-2 days
**Objective**: Establish basic project structure and development environment

#### Tasks
1. **Project Initialization**
   - Create TypeScript web application structure
   - Set up build system (Webpack, TypeScript)
   - Configure development environment

2. **VEX Extension Configuration**
   - Install and configure VEX VS Code Extension
   - Enable WebSocket Server feature
   - Document Device ID and connection parameters

3. **Basic Communication Testing**
   - Verify WebSocket connection between web interface and VEX Extension
   - Test serial communication between VEX Extension and VEX brain
   - Establish baseline communication reliability

#### Deliverables
- Functional web application skeleton
- Working WebSocket connection to VEX Extension
- Basic serial communication on VEX brain
- Development and testing documentation

### Phase 2: Core Voice Control Implementation
**Duration**: 2-3 days
**Objective**: Implement basic voice recognition and command processing

#### Tasks
1. **Speech Recognition Integration**
   - Implement Web Speech API integration
   - Configure continuous voice recognition
   - Handle speech recognition errors and edge cases

2. **Command Processing System**
   - Define voice command vocabulary
   - Implement command validation and mapping
   - Create command execution pipeline

3. **Basic Robot Control**
   - Implement movement commands (forward, backward, left, right, stop)
   - Add mechanism control (intake, score)
   - Ensure immediate robot response

#### Deliverables
- Functional voice recognition system
- Complete command processing pipeline
- Basic robot movement and mechanism control
- Error handling for voice recognition failures

### Phase 3: Advanced Features and Status Reporting
**Duration**: 2-3 days
**Objective**: Add status reporting and enhance user experience

#### Tasks
1. **Status Reporting System**
   - Implement robot status collection (heading, distance)
   - Create status message format and transmission
   - Add real-time status display on web interface

2. **Enhanced User Interface**
   - Design intuitive web interface layout
   - Add connection status indicators
   - Implement command history and feedback

3. **Continuous Operation**
   - Enable continuous voice recognition
   - Implement automatic reconnection
   - Add system health monitoring

#### Deliverables
- Real-time status reporting system
- Enhanced web interface with status display
- Continuous voice recognition operation
- System health monitoring and feedback

### Phase 4: Testing and Optimization
**Duration**: 1-2 days
**Objective**: Comprehensive testing and performance optimization

#### Tasks
1. **Functional Testing**
   - Test all voice commands and robot responses
   - Verify status reporting accuracy
   - Test error handling and recovery

2. **Performance Optimization**
   - Optimize voice recognition accuracy
   - Reduce command response latency
   - Improve system reliability

3. **User Experience Testing**
   - Conduct usability testing
   - Gather feedback and iterate
   - Document best practices

#### Deliverables
- Comprehensive test suite
- Performance benchmarks
- User experience documentation
- Deployment-ready system

## Voice Command Specification

### Supported Commands

#### Movement Commands
- **"move"**: Drive robot forward at moderate speed
- **"back"**: Drive robot backward at moderate speed  
- **"left"**: Turn robot left (differential drive)
- **"right"**: Turn robot right (differential drive)
- **"stop"**: Stop all robot movement and report status

#### Mechanism Commands
- **"roll"**: Activate intake mechanism (continuous operation)
- **"shoot"**: Activate scoring mechanism (continuous operation)

### Command Characteristics
- **Single-word commands**: Simple, clear vocabulary for reliable recognition
- **Immediate execution**: Commands execute as soon as recognized
- **Continuous operation**: Mechanism commands run until stop command
- **Status feedback**: Stop command includes robot status report

### Command Mapping
Voice commands are mapped to complete word serial commands for reliable transmission:
- "move" → "FORWARD"
- "back" → "BACKWARD" 
- "left" → "LEFT"
- "right" → "RIGHT"
- "stop" → "STOP"
- "roll" → "ROLL"
- "shoot" → "SHOOT"

## Technical Requirements

### Hardware Requirements
- VEX V5 Brain with motors and sensors
- VEX Controller for manual override
- USB cable for brain/controller connection
- Computer with microphone and internet connection

### Software Requirements
- VEX VS Code Extension with WebSocket Server enabled
- Modern web browser with Web Speech API support
- Node.js development environment
- TypeScript compiler and build tools

### Communication Requirements
- Local network WebSocket connection
- USB serial communication between VEX Extension and brain
- Real-time bidirectional message exchange
- Reliable error detection and recovery

## Serial Communication Strategy

### Communication Protocol
The system uses a robust serial communication protocol through the RemoteControl module:

#### Command Transmission
- **Complete word commands**: "FORWARD", "STOP", "LEFT", etc.
- **Line-based processing**: Commands end with newline character (`\n`)
- **Case-insensitive**: Accept both uppercase and lowercase characters
- **Error handling**: Graceful handling of invalid commands

#### Status Reporting
- **Triggered by stop command**: Status only sent when requested
- **Standard format**: `STATUS:heading:distance` with consistent formatting
- **Real-time transmission**: Immediate response to status requests
- **Error recovery**: Fallback values for sensor failures

### Serial Port Reading Implementation

#### File-Based Serial Communication
The RemoteControl module uses file-based serial communication on the VEX brain:

```cpp
// Open serial port for reading
FILE* serialFile = fopen("/dev/serial1", "rb");
```

#### Character-by-Character Reading
Commands are read one character at a time to build complete lines:

```cpp
// Read one character at a time
char c;
if (fread(&c, 1, 1, serialFile) == 1) {
    if (c == '\n' || c == '\r') {
        // Process complete line
        processCommand(lineBuffer);
        lineBuffer.clear();
    } else {
        // Add character to buffer
        lineBuffer += c;
    }
}
```

#### Line Buffer Management
- **Static buffer**: `std::string lineBuffer` accumulates characters
- **Newline detection**: Process commands when `\n` or `\r` is encountered
- **Empty line filtering**: Only process non-empty command lines
- **Buffer clearing**: Reset buffer after processing each command

#### Command Processing
- **Whitespace removal**: Strip leading/trailing whitespace and newlines
- **Case conversion**: Convert to uppercase for case-insensitive comparison
- **Command mapping**: Map to robot actions (FORWARD, STOP, etc.)
- **Debug output**: Show processed command on controller screen

### Implementation Approach
- **Polling-based**: Check for new commands every 100ms in main loop
- **Non-blocking**: Serial operations don't interfere with robot control
- **Robust error handling**: Connection failures don't crash the system
- **User feedback**: Controller screen and rumble provide immediate feedback

#### Polling Integration
```cpp
// In main.cpp
while (true) {
    if (remoteListening) {
        remoteControl.poll();  // Check for new commands
    }
    wait(100, msec);
}
```

#### Button Control
- **Button X toggle**: Press to enable/disable serial listening
- **Connection status**: Display on controller screen
- **Error feedback**: Rumble patterns for different states

## User Interface Design

### Web Interface Layout
- **Connection section**: Device ID input and connection status
- **Voice control section**: Start/stop listening with status indicators
- **Command display**: Dynamic list of recognized commands
- **Status display**: Real-time robot heading and distance
- **Activity log**: Command history and system messages

### User Experience Goals
- **Intuitive operation**: Minimal learning curve for new users
- **Clear feedback**: Visual and audio confirmation of actions
- **Reliable operation**: Consistent behavior across different conditions
- **Accessible design**: Support for different user needs and preferences

## Testing Strategy

### Functional Testing
- **Voice command recognition**: Test accuracy across different speakers
- **Robot response**: Verify all commands execute correctly
- **Status reporting**: Validate accuracy of heading and distance data
- **Error handling**: Test system behavior under various failure conditions

### Performance Testing
- **Response time**: Measure command-to-action latency
- **Recognition accuracy**: Test voice recognition reliability
- **System stability**: Long-term operation testing
- **Resource usage**: Monitor memory and CPU utilization

### User Acceptance Testing
- **Usability testing**: Observe users interacting with the system
- **Feedback collection**: Gather user suggestions and concerns
- **Iterative improvement**: Refine system based on user feedback
- **Documentation**: Create user guides and troubleshooting information

## Risk Assessment and Mitigation

### Technical Risks
- **Voice recognition accuracy**: Implement fallback commands and error handling
- **Communication reliability**: Add reconnection logic and error recovery
- **System performance**: Monitor and optimize resource usage
- **Hardware compatibility**: Test across different VEX configurations

### User Experience Risks
- **Learning curve**: Provide clear documentation and intuitive interface
- **Environmental factors**: Test in various acoustic environments
- **User expectations**: Set realistic performance expectations
- **Accessibility**: Ensure system works for users with different needs

## Success Metrics

### Performance Metrics
- Voice command recognition accuracy > 90%
- Command response time < 500ms
- System uptime > 95%
- Status reporting accuracy within 5%

### User Experience Metrics
- User satisfaction score > 4.0/5.0
- Time to first successful command < 2 minutes
- Error rate < 5% during normal operation
- User retention rate > 80%

## Future Enhancements

### Potential Improvements
- **Advanced voice commands**: Multi-word commands and parameters
- **Autonomous integration**: Voice-triggered autonomous routines
- **Mobile support**: Progressive web app for mobile devices
- **Multi-robot support**: Control multiple robots simultaneously
- **Machine learning**: Adaptive voice recognition for individual users

### Scalability Considerations
- **Modular architecture**: Easy addition of new commands and features
- **Configuration management**: Flexible system configuration
- **Plugin system**: Extensible command processing framework
- **API design**: Clean interfaces for future integrations

## Conclusion

This voice control system provides an innovative way to interact with VEX robots, combining the power of modern web technologies with the reliability of VEX hardware. The phased implementation approach ensures steady progress while maintaining system quality and user experience.

The system's modular design and comprehensive testing strategy provide a solid foundation for future enhancements and adaptations to different use cases. By focusing on reliability, usability, and performance, this voice control system will provide an excellent user experience for robot control applications. 