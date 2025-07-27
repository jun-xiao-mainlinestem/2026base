# Voice-Controlled Robot

A web-based voice control interface for VEX V5 robots using WebSocket communication through the VEX VS Code Extension.

## Features

- 🎤 **Voice Recognition**: Real-time speech-to-command conversion
- 🔌 **WebSocket Communication**: Direct connection to VEX Extension
- 📊 **Real-time Status**: Live robot heading and distance display
- 🎮 **Simple Commands**: Intuitive voice commands for robot control
- 📱 **Responsive Design**: Works on desktop and mobile devices

## Supported Commands

- **move** - Drive forward using voltage control
- **back** - Drive backward using voltage control
- **left** - Turn left using voltage control
- **right** - Turn right using voltage control
- **stop** - Stop robot and report status
- **roll** - Activate intake mechanism (continues until stop)
- **shoot** - Activate scoring mechanism (continues until stop)

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
3. **Enter your Device ID** in the input field
4. **Connect to robot**: Click "Connect to Robot"
5. **Start listening**: Click "Start Listening"
6. **Speak commands**: Use the supported voice commands (e.g., "move", "stop", "left", "roll", "shoot")

## VEX Code Integration

The VEX brain code must be updated to handle the voice commands. The system uses single-character commands sent via WebSocket through the VEX Extension.

### Key Components
- **Serial Communication**: Reads single-character commands from `/dev/serial1`
- **Command Mapping**: Maps characters to robot actions
- **Status Reporting**: Sends robot status when "stop" command is received

### Command Mapping
- `a` → FORWARD
- `b` → BACKWARD
- `l` → LEFT
- `d` → RIGHT
- `p` → STOP
- `i` → INTAKE
- `s` → SCORE

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
├── test_websocket_simple.js # WebSocket test script
└── README.md
```

## Testing

### WebSocket Test
Use the included test script to verify WebSocket communication:

```bash
cd voice-control-robot
node test_websocket_simple.js [DEVICE_ID]
```

This will send a sequence of test commands to verify robot response.

## Troubleshooting

### Common Issues

1. **WebSocket Connection Failed**
   - Check VEX Extension settings
   - Verify Device ID is correct
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