# Voice-Controlled Robot

A web-based voice control interface for VEX V5 robots using WebSocket communication through the VEX VS Code Extension.

## Features

- 🎤 **Voice Recognition**: Real-time speech-to-command conversion
- 🔌 **WebSocket Communication**: Direct connection to VEX Extension
- 📊 **Real-time Status**: Live robot heading and distance display
- 🎮 **Simple Commands**: Intuitive voice commands for robot control
- 📱 **Responsive Design**: Works on desktop and mobile devices

## Supported Commands

- **forward** - Drive forward using voltage control
- **backward/back** - Drive backward using voltage control
- **left** - Turn left using voltage control
- **right** - Turn right using voltage control
- **stop** - Stop robot and report status
- **intake** - Activate intake mechanism (continues until stop)
- **score** - Activate scoring mechanism (continues until stop)

## Prerequisites

### Hardware
- VEX V5 Brain
- VEX Controller
- USB cable for brain/controller connection
- Computer with microphone

### Software
- VEX VS Code Extension
- Node.js (v14 or higher)
- Modern web browser with Web Speech API support

### VEX Extension Setup
1. Install VEX VS Code Extension
2. Connect VEX brain/controller via USB
3. Enable WebSocket Server in VEX Extension settings
4. Note the Device ID and WebSocket port

## Installation

1. **Clone or download the project**
   ```bash
   git clone <repository-url>
   cd voice-control-robot
   ```

2. **Install dependencies**
   ```bash
   npm install
   ```

3. **Configure your device**
   - Open `src/app.ts`
   - Update the WebSocket configuration:
     ```typescript
     this.websocket = new VEXWebSocket({
       id: "YOUR_DEVICE_ID", // Replace with your device ID
       port: "7071",         // Update if different
       ip: "localhost"
     });
     ```

## Usage

### Development Mode
```bash
npm run dev
```
This will start the development server at `http://localhost:3000`

### Production Build
```bash
npm run build
```
This creates optimized files in the `dist/` directory

### Running the Application
1. **Start the development server**: `npm run dev`
2. **Open your browser** to `http://localhost:3000`
3. **Connect to robot**: Click "Connect to Robot"
4. **Start listening**: Click "Start Listening"
5. **Speak commands**: Use the supported voice commands

## VEX Code Integration

The VEX brain code must be updated to handle the voice commands. Add this to your `main.cpp`:

```cpp
// In buttonX_action() function
serialComm.onMessage([](const std::string& message) {
    controller(primary).Screen.print("Received: %s", message.c_str());
    
    if (message == "FORWARD") {
        chassis.drive_with_voltage(6, 6);
    }
    else if (message == "BACKWARD") {
        chassis.drive_with_voltage(-6, -6);
    }
    else if (message == "LEFT") {
        chassis.drive_with_voltage(-6, 6);
    }
    else if (message == "RIGHT") {
        chassis.drive_with_voltage(6, -6);
    }
    else if (message == "STOP") {
        chassis.stop(brake);
        stop_rollers();
        
        // Send status back
        float current_heading = chassis.get_heading();
        float distance_traveled = (chassis.get_left_position_in() + chassis.get_right_position_in()) / 2.0;
        
        std::string status_message = "STATUS:" + std::to_string(current_heading) + ":" + std::to_string(distance_traveled) + "\n";
        serialComm.send(status_message);
    }
    else if (message == "INTAKE") {
        in_take();
    }
    else if (message == "SCORE") {
        score_long();
    }
});
```

## Project Structure

```
voice-control-robot/
├── src/
│   ├── index.html          # Main web interface
│   ├── styles.css          # Styling
│   ├── app.ts              # Main TypeScript application
│   ├── websocket.ts        # WebSocket communication
│   ├── speech.ts           # Speech recognition
│   ├── commands.ts         # Command processing
│   └── types.ts            # TypeScript type definitions
├── dist/                   # Compiled JavaScript
├── package.json
├── tsconfig.json
├── webpack.config.js
└── README.md
```

## Troubleshooting

### Common Issues

1. **WebSocket Connection Failed**
   - Check VEX Extension settings
   - Verify Device ID in `app.ts`
   - Ensure USB connection

2. **Speech Recognition Not Working**
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

## Browser Compatibility

- Chrome/Chromium (recommended)
- Firefox
- Safari (limited support)
- Edge

## Security Notes

- WebSocket server runs on localhost only
- No authentication required (intended for local development)
- All commands validated before execution

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## License

This project is licensed under the ISC License.

## Support

For issues and questions:
1. Check the troubleshooting section
2. Review VEX Extension documentation
3. Check browser compatibility
4. Verify hardware connections 