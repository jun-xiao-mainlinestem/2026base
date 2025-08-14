# RGB Remote Control - Simple Version

A simple, single-file HTML web application for controlling VEX robots remotely.

## Features

- **Single HTML file** - no build process, no dependencies
- **WebSocket connection** to VEX robot
- **Three command types**: Drive, Turn, Set Heading
- **Enable/disable interface** - command section only active when connected
- **Minimal styling** - clean and functional

## How to Use

### 1. Start the Web Server

```bash
cd RGB_remote_simple
python3 -m http.server 8000
```

### 2. Open in Browser

Navigate to: `http://localhost:8000`

### 3. Connect to Robot

1. Enter your robot's Device ID
2. Click "Connect"
3. Wait for connection status to show "Connected"

### 4. Send Commands

Once connected, the command section becomes active:

- **Drive**: Enter distance in inches (negative for backward)
- **Turn**: Enter target heading (0-360°)
- **Set Heading**: Set current heading (0-360°)

## Command Format

The app sends these commands to your robot:

- `drive <distance>\n`
- `turn <heading>\n`
- `set_heading <heading>\n`

## Requirements

- **Web Server**: Python 3 (or any HTTP server)
- **Robot**: VEX V5 with WebSocket server running on port 7071
- **Browser**: Modern web browser with WebSocket support

## File Structure

```
RGB_remote_simple/
└── index.html          # Single file with everything embedded
```

## No Dependencies

- ❌ No Node.js
- ❌ No npm packages
- ❌ No build tools
- ❌ No TypeScript compilation
- ✅ Pure HTML/CSS/JavaScript

## Deployment

Simply upload the `index.html` file to any web server or hosting service.
