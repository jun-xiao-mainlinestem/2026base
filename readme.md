# Voice-Controlled VEX V5 Robot

A complete voice control system for VEX V5 robots using web technologies and WebSocket communication through the VEX VS Code Extension.

## 🎯 Project Overview

This project enables voice control of VEX V5 robots through a web interface. Users can speak commands like "forward", "stop", "left", etc., and the robot will execute the corresponding actions in real-time.

## 🏗️ Architecture

```
Web Interface (TypeScript) → WebSocket → VEX Extension → Serial → VEX Brain → Robot
```

### Components
- **Web Interface**: Modern TypeScript/Webpack application with speech recognition
- **VEX Extension**: Bridges WebSocket communication to VEX brain
- **VEX Brain**: C++ code that processes commands and controls robot mechanisms
- **Serial Communication**: Direct USB communication between computer and VEX brain

## 📁 Project Structure

```
2026base_jun/
├── voice-control-robot/           # Web interface application
│   ├── src/                      # TypeScript source files
│   ├── dist/                     # Compiled output
│   ├── package.json              # Node.js dependencies
│   └── README.md                 # Web interface documentation
├── src/                          # VEX C++ source files
│   ├── main.cpp                  # Main robot program
│   ├── serial_communication.cpp  # Serial communication handling
│   ├── chassis.cpp               # Drivetrain control
│   ├── autons.cpp                # Autonomous routines
│   └── robot-config.cpp          # Hardware configuration
├── include/                      # C++ header files
│   ├── serial_communication.h    # Serial communication interface
│   ├── chassis.h                 # Drivetrain interface
│   ├── autons.h                  # Autonomous interface
│   └── robot-config.h            # Hardware configuration interface
├── doc/                          # Documentation
│   ├── voice_control_plan.md     # Original implementation plan
│   ├── voice_control_implementation.md # Complete implementation guide
│   └── ARCHITECTURE.md           # System architecture details
└── readme.md                     # This file
```

## 🚀 Quick Start

### Prerequisites
- VEX V5 Brain + Controller
- VEX VS Code Extension
- Node.js (v14+)
- Computer with microphone

### Setup Steps

1. **VEX Extension Setup**
   ```bash
   # Install VEX VS Code Extension
   # Connect VEX brain via USB
   # Enable WebSocket Server in extension settings
   # Note your Device ID
   ```

2. **Web Interface Setup**
   ```bash
   cd voice-control-robot
   npm install
   npm run dev
   ```

3. **VEX Code Upload**
   ```bash
   # Upload main.cpp to VEX brain
   # Press Button X on controller to enable listening
   ```

4. **Start Voice Control**
   ```bash
   # Open http://localhost:3000
   # Enter Device ID and connect
   # Start listening and speak commands
   ```

## 🎤 Voice Commands

| Command | Action | Character Code |
|---------|--------|----------------|
| "forward" | Drive forward | `a` |
| "backward" | Drive backward | `b` |
| "left" | Turn left | `l` |
| "right" | Turn right | `d` |
| "stop" | Stop and report status | `p` |
| "intake" | Start intake mechanism | `i` |
| "score" | Start scoring mechanism | `s` |

## 🔧 Testing

### WebSocket Test
```bash
cd voice-control-robot
node test_websocket_simple.js [DEVICE_ID]
```

### Manual Testing
1. Press Button X on VEX controller
2. Verify "Serial listening ON" appears
3. Use web interface or test script to send commands
4. Check robot movement and controller screen

## 📚 Documentation

- **[Web Interface README](voice-control-robot/README.md)** - Complete web application guide
- **[Implementation Guide](doc/voice_control_implementation.md)** - Detailed technical implementation
- **[Quick Start Guide](voice-control-robot/QUICKSTART.md)** - 5-minute setup guide
- **[Test Guide](voice-control-robot/test_robot_commands.md)** - Testing procedures
- **[Architecture](doc/ARCHITECTURE.md)** - System design details

## 🛠️ Development

### Web Interface Development
```bash
cd voice-control-robot
npm run dev          # Development server
npm run build        # Production build
npm run test         # Run tests
```

### VEX Code Development
- Edit C++ files in `src/` and `include/`
- Upload to VEX brain via VEX Extension
- Test with Button X functionality

## 🔍 Troubleshooting

### Common Issues
1. **WebSocket Connection Failed**
   - Check VEX Extension settings
   - Verify Device ID is correct
   - Ensure USB connection

2. **Voice Recognition Not Working**
   - Check microphone permissions
   - Use HTTPS (required for some browsers)
   - Test with different browsers

3. **Robot Not Responding**
   - Verify Button X is pressed on VEX controller
   - Check serial communication
   - Review command mapping

### Debug Tools
- Browser Developer Tools (F12)
- VEX Extension logs
- Serial communication monitor
- Controller screen messages

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## 📄 License

This project is licensed under the ISC License.

## 🆘 Support

For issues and questions:
1. Check the troubleshooting sections in documentation
2. Review VEX Extension documentation
3. Check browser compatibility
4. Verify hardware connections

---

**Ready to control your VEX robot with voice commands!** 🎤🤖 