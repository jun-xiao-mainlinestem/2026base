# VEX Robot Command Line Interface

A web-based command line interface for controlling VEX robots through WebSocket communication.

## Features

- **Real-time Command Input**: Type commands directly in a terminal-like interface
- **WebSocket Communication**: Connects to VEX VS Code Extension WebSocket server
- **Comprehensive Command Set**: Support for drive, turn commands
- **Command History**: View all sent commands and robot responses
- **Status Monitoring**: Real-time connection status and robot feedback
- **Deployable Website**: Can be deployed to Netlify or other hosting platforms

## Setup

### Local Development

1. **Install Dependencies**:
   ```bash
   npm install
   ```

2. **Start Development Server**:
   ```bash
   npm run dev
   ```

3. **Build for Production**:
   ```bash
   npm run build
   ```

### Deployment to Netlify

1. **Connect to Netlify**:
   - Push your code to GitHub
   - Connect your repository to Netlify
   - Set build command: `npm run build`
   - Set publish directory: `dist`

2. **Environment Variables** (if needed):
   - Add any environment variables in Netlify dashboard

3. **Deploy**:
   - Netlify will automatically build and deploy your site

## Available Commands

### Drive Commands
```
drive <distance>                    # Drive forward/backward (inches)
drive <distance> <voltage>          # Drive with custom voltage
drive <distance> <voltage> <heading> <heading-voltage>  # Drive while maintaining heading
```

**Examples:**
- `drive 24` - Drive 24 inches forward
- `drive -12` - Drive 12 inches backward
- `drive 24 8` - Drive 24 inches with 8V
- `drive 24 10 45 6` - Drive 24 inches while maintaining 45° heading

### Turn Commands
```
turn <heading>                      # Turn to heading (degrees)
turn <heading> <voltage>            # Turn with custom voltage
```

**Examples:**
- `turn 90` - Turn to 90 degrees
- `turn 180 8` - Turn to 180 degrees with 8V

## WebSocket Message Format

The interface sends commands to the robot in the following format:

### Drive Commands
- `DRIVE:{distance}:{voltage}\n` - Basic drive
- `DRIVE:{distance}:{voltage}:{heading}:{heading-voltage}\n` - Drive with heading

### Turn Commands
- `TURN:{heading}:{voltage}\n` - Turn to heading

## Robot Response Format

The robot responds with status messages:
- `STATUS:{heading}:{distance}\n` - Robot status after stop command

## Usage

1. **Connect to Robot**:
   - Enter your VEX Device ID
   - Click "Connect to Robot"
   - Wait for connection confirmation

2. **Send Commands**:
   - Type commands in the command line window
   - Press Enter or click "Send"
   - View command history and responses

## Project Structure

```
command_line_control/
├── src/
│   ├── index.html          # HTML template
│   ├── styles.css          # CSS styling
│   ├── app.ts              # Main application logic
│   ├── websocket.ts        # WebSocket communication
│   └── types.ts            # TypeScript type definitions
├── package.json            # Dependencies and scripts
├── webpack.config.js       # Webpack configuration
├── tsconfig.json           # TypeScript configuration
├── netlify.toml           # Netlify deployment config
└── README.md              # This file
```

## Development

### Adding New Commands

1. **Update `types.ts`**:
   ```typescript
   export interface ParsedCommand {
     type: 'drive' | 'turn' | 'new-command';
     // ... other properties
   }
   ```

2. **Update `app.ts`**:
   - Add parsing logic in `parseCommand()`
   - Add formatting logic in `formatRobotCommand()`

3. **Update HTML**:
   - Add command documentation in the help section

### Building and Testing

```bash
# Development
npm run dev

# Production build
npm run build

# Serve production build
npm start
```

## Technical Details

- **Framework**: TypeScript + Webpack
- **WebSocket URL**: `ws://localhost:7071/vexrobotics.vexcode/device?id={device-id}`
- **Message Format**: Text-based with newline termination
- **Deployment**: Netlify-ready with proper configuration
- **Browser Support**: Modern browsers with WebSocket API

## Troubleshooting

### Build Issues
- Ensure Node.js version 16+ is installed
- Run `npm install` to install dependencies
- Check TypeScript configuration in `tsconfig.json`

### Connection Issues
- Verify VEX VS Code Extension is running
- Check Device ID is correct
- Ensure WebSocket server is active on port 7071

### Deployment Issues
- Verify `netlify.toml` configuration
- Check build command and publish directory
- Ensure all dependencies are in `package.json`



