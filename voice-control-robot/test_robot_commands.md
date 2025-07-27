# Robot Command Test Guide

## Quick Test: Manual Commands

### Step 1: Enable Serial Communication
1. **Press Button X** on VEX controller
2. **Verify** controller screen shows: `"Serial listening ON"`
3. **Listen for** controller rumble sound

### Step 2: Test Manual Commands
Use the Node.js test script to send commands directly:

```bash
cd voice-control-robot
node test_websocket_simple.js
```

The script will prompt for your device ID or you can provide it as an argument:
```bash
node test_websocket_simple.js 77606
```

The script will send these single-character commands automatically:
- `a` (MOVE) - robot should move forward
- `p` (STOP) - robot should stop
- `d` (RIGHT) - robot should turn right
- `l` (LEFT) - robot should turn left
- `b` (BACK) - robot should move backward
- `i` (ROLL) - robot should start intake
- `s` (SHOOT) - robot should start scoring

### Step 3: Check Controller Screen
For each command, you should see:
```
MOVE
STOP
RIGHT
LEFT
BACK
ROLL
SHOOT
```

### Step 4: Check Robot Movement
- **MOVE (a)**: Robot drives forward
- **BACK (b)**: Robot drives backward  
- **LEFT (l)**: Robot turns left
- **RIGHT (d)**: Robot turns right
- **STOP (p)**: Robot stops and reports status
- **ROLL (i)**: Robot starts intake mechanism
- **SHOOT (s)**: Robot starts scoring mechanism

## If Manual Commands Don't Work:

### Check 1: Button X Status
- Press Button X multiple times
- Look for `"Serial listening ON"` message
- Listen for rumble sound when pressed

### Check 2: USB Connection
- Ensure VEX brain is connected via USB
- Check VS Code shows connected device

### Check 3: Code Upload
- Re-upload the main.cpp code to VEX brain
- Restart the VEX brain if needed

### Check 4: Voltage Values
- If robot moves but very slowly, voltage might be too low
- Current setting: 6V (should be sufficient)

## If Manual Commands Work but Voice Doesn't:

### Check 1: WebSocket Connection
- Verify web interface shows "🟢 Connected"
- Check browser console for connection errors

### Check 2: Command Format
- Voice commands should send single characters: `a`, `p`, `d`, `l`, `b`, `i`, `s`
- Check VS Code terminal shows commands being received

### Check 3: Timing
- There might be a delay between voice command and robot response
- Try waiting 1-2 seconds between commands

## Testing Serial Command Reception via WebSocket

This test verifies that your VEX robot can receive and execute single-character commands sent via the VEX VS Code Extension's WebSocket server.

### Prerequisites
- The robot is powered on and connected to your computer via USB.
- The VEX VS Code Extension is running and its WebSocket server is enabled.
- The robot program is running and ready to receive serial commands (press X on the controller to enable serial listening).
- Node.js is installed on your computer.

### How to Run the Test

1. **Navigate to the test script directory:**
   ```bash
   cd voice-control-robot
   ```

2. **Run the test script with your device ID:**
   ```bash
   node test_websocket_simple.js 77606
   ```
   Replace `77606` with your actual VEX device ID if different.

   - **Alternatively**, if you omit the device ID, the script will prompt you:
     ```bash
     node test_websocket_simple.js
     ```
     Then enter your device ID when prompted, or press Enter to use the default (`77606`).

3. **Observe the robot and controller:**
   - The script will send a sequence of single-character commands:
     - `a` (MOVE)
     - `p` (STOP)
     - `d` (RIGHT)
     - `l` (LEFT)
     - `b` (BACK)
     - `i` (ROLL)
     - `s` (SHOOT)
   - Each command is sent with a short delay.
   - The robot should move or act according to each command, and the controller screen should display the current action.

4. **Test completion:**
   - After the sequence, the script will close the connection and print a summary.
   - You can re-run the test as needed.

### Example Output
```
🔧 Simple WebSocket Test (Single-Character Commands)
==================================
Connecting to: ws://localhost:7071/vexrobotics.vexcode/device?id=77606
⏳ Waiting for connection...
✅ WebSocket connected!
📤 Sending: FORWARD (char: 'a')
📤 Sending: STOP (char: 'p')
... (other commands)
✅ Test sequence completed!
🔌 WebSocket closed. Code: 1000, Reason: 
📊 Test Results:
   WebSocket Connected: ✅
   Message Sent: ✅
   Message Received: ✅
💡 Check the VEX controller screen and robot movement for each command.
```

### Troubleshooting
- Make sure the robot is in "Serial listening ON" mode (press X on the controller).
- Ensure the correct device ID is used.
- Confirm the VEX VS Code Extension WebSocket server is running.
- If the robot does not respond, check the controller screen for error messages. 

### Check 3: Robot Not Moving
- Check controller screen for "Serial listening ON"
- Verify commands in activity log
- Test with "stop" command first 