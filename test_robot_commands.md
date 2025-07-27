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
node test_vex_serial.js
```

Then type these commands one by one:
- `FORWARD` (robot should move forward)
- `STOP` (robot should stop)
- `LEFT` (robot should turn left)
- `STOP` (robot should stop)
- `RIGHT` (robot should turn right)
- `STOP` (robot should stop)

### Step 3: Check Controller Screen
For each command, you should see:
```
Received: FORWARD
Executing: FORWARD
```

### Step 4: Check Robot Movement
- **FORWARD**: Robot drives forward
- **BACKWARD**: Robot drives backward  
- **LEFT**: Robot turns left
- **RIGHT**: Robot turns right
- **STOP**: Robot stops and reports status

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
- Voice commands should send: `FORWARD`, `STOP`, etc.
- Check VS Code terminal shows commands being received

### Check 3: Timing
- There might be a delay between voice command and robot response
- Try waiting 1-2 seconds between commands 