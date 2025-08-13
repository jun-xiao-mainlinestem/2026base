# RGB Remote Control

A modern, mobile-friendly web application for remote control of VEX robots through websocket connections.

## Features

- **Connection Management**: Connect to robots using IP address and device ID
- **Command Interface**: Send drive, turn, and set heading commands
- **Parameter Validation**: Automatic validation of command parameters
- **Command History**: Track all sent commands with success/failure status
- **Mobile-First Design**: Optimized for touch devices and mobile browsers
- **Real-time Status**: Live connection status and feedback

## Commands Supported

### Drive Command
```
Format: drive <distance> <voltage> <heading>
Parameters:
- distance: integer (inches, can be negative)
- voltage: integer (-12 to +12V, default: 10)
- heading: integer (0-360°, default: 0)
Example: drive 12 10 90
```

### Turn Command
```
Format: turn <heading> <voltage>
Parameters:
- heading: integer (0-360°)
- voltage: integer (-12 to +12V, default: 10)
Example: turn 180 8
```

### Set Heading Command
```
Format: set_heading <heading>
Parameters:
- heading: integer (0-360°)
Example: set_heading 270
```

## Setup

### Prerequisites
- Node.js 16+ and npm
- Modern web browser with WebSocket support

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

1. Enter the robot's IP address (e.g., `192.168.1.100`)
2. Enter a device ID (e.g., `robot001`)
3. Click "Connect"

**Note:** The application uses port 7071 by default for WebSocket connections.

### Sending Commands

1. Select the command type from the dropdown
2. Fill in the required parameters
3. Click "Send Command"

### Command History

All sent commands are displayed in the history section with:
- Timestamp
- Command text
- Success/failure status

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

### Key Components

- **WebSocketManager**: Handles robot connections and communication
- **UIManager**: Manages user interface updates and interactions
- **CommandValidator**: Validates command parameters
- **CommandFormatter**: Formats commands for transmission

### Building and Deployment

The application is built using Webpack 5 and TypeScript. The production build creates optimized files in the `dist/` folder that can be deployed to any static hosting service like Netlify.

## Browser Support

- **Mobile**: iOS Safari 12+, Chrome Mobile 70+, Samsung Internet 10+
- **Desktop**: Chrome 70+, Firefox 65+, Safari 12+, Edge 79+
- **Minimum**: ES2017+ (async/await support)

## Deployment

### Netlify
1. Build the project: `npm run build`
2. Deploy the `dist/` folder to Netlify
3. Set build command: `npm run build`
4. Set publish directory: `dist/`

### Other Static Hosting
The `dist/` folder contains all necessary files for deployment to any static hosting service.

## Troubleshooting

### Connection Issues
- Verify the robot's IP address is correct
- Ensure the robot is running the websocket server on port 7071
- Verify the device ID matches the robot's configuration

### Command Issues
- Verify all parameters are within valid ranges
- Check the command history for error messages
- Ensure the robot is connected before sending commands

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## License

MIT License - see LICENSE file for details.
