# RGB Remote Control

A sample web application for remote control of VEX robots through websocket connections.

## Setup VEX Extension
1. Open VS Code with VEX Extension
2. **Configure WebSocket Server**:
   - Go to VEX Extension settings: https://kb.vex.com/hc/en-us/articles/21410932256660-Understanding-the-User-Settings-for-VEX-VS-Code-Extension
   - Enable WebSocket Server in extension settings
3. **Get your Device ID**:
   - View brain info: https://kb.vex.com/hc/en-us/articles/8608865978644-VS-Code-UI-Overview-for-V5
   - Note your Device ID (process ID) - this is your unique identifier
   - The WebSocket URL format is: `ws://localhost:7071/vexrobotics.vexcode/device?id=YOUR_DEVICE_ID`


## Setup Web Application

### Prerequisites
- Node.js 16+ and npm

### Installation

1. **Install dependencies**:
   ```bash
   npm install
   ```

2. **Development mode**:
   ```bash
   npm run dev
   ```
   This will start the development server at `http://localhost:8080`

3. **Production build**:
   ```bash
   npm run build
   ```
   This creates optimized files in the `dist/` folder

## Usage

### Connecting to a Robot

1. Enter the device ID (e.g., `5567`)
2. Click "Connect"

**Note:** The application connects to `localhost:7071` by default for WebSocket connections.

### Sending Commands

1. Select the command type from the dropdown
2. Fill in the required parameters
3. Click "Send Command"

### Command History

All sent commands are displayed in the history section with:
- Timestamp
- Command text

## Development

### Project Structure
```
src/
├── app.ts                 # Main application logic
├── websocket.ts           # WebSocket connection management
├── commands.ts            # Command validation and formatting
├── ui.ts                  # UI management and updates
├── types.ts               # TypeScript type definitions
├── index.html             # HTML template
└── styles.css             # CSS styles
```

### Building and Deployment

The application is built using Webpack 5 and TypeScript. The production build creates optimized files in the `dist/` folder that can be deployed to any static hosting service like Netlify.

## Troubleshooting

### Connection Issues
- Verify the robot's IP address is correct
- Verify the device ID is correct
- Ensure the VEX extension is running the websocket server on port 7071
- Ensure the mobile device is on the same network as the computer connnected to the controller
- Verify USB connection

