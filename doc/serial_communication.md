# Serial Communication Documentation

This document describes the serial communication implementation for reading messages from the VEX brain's user port (USB).

## Overview

The serial communication module provides an interface for reading and writing messages through the VEX V5 brain's USB port. This allows communication with a computer or other devices connected via USB.

## Features

- Read messages from USB port
- Send messages to USB port
- Message callback system
- Buffer management
- Line-by-line message processing

## Usage

### Basic Example

```cpp
#include "serial_communication.h"
using namespace serial;

// Create a serial communication object
SerialCommunication serial;

// Set up message callback
serial.onMessage([](const std::string& message) {
    // Handle incoming messages
    controller(primary).Screen.print("Received: %s", message.c_str());
});

// Connect to serial communication
if (serial.connect()) {
    // Send a message
    serial.send("Hello, computer!\n");
    
    // Poll for messages in main loop
    while (serial.isConnected()) {
        serial.poll();
        wait(50, msec);
    }
}
```

### API Reference

#### Constructor
```cpp
SerialCommunication();
```

#### Connection Methods
- `bool connect()` - Establishes serial communication
- `void disconnect()` - Closes the connection
- `bool isConnected()` - Returns connection status

#### Communication Methods
- `bool send(const std::string& message)` - Sends a message
- `void poll()` - Processes incoming messages (call in main loop)
- `std::string readLine()` - Reads one line from buffer
- `std::string readAll()` - Reads all available data

#### Callback Setters
- `onMessage(callback)` - Called when a message is received
- `onError(callback)` - Called when an error occurs

#### Utility Methods
- `void clearBuffer()` - Clears the message buffer
- `bool hasData()` - Returns true if data is available
- `int availableBytes()` - Returns number of bytes in buffer

## Communication Protocol

### Message Format
Messages are expected to be line-delimited (ending with `\n`). The system automatically processes messages line by line.

### Example Messages
```
MOVE_FORWARD 100
TURN_LEFT 90
STOP
STATUS_REQUEST
```

## Computer Side Setup

To communicate with the VEX brain from a computer, you can use various tools:

### Python Example
```python
import serial
import time

# Connect to VEX brain (adjust port as needed)
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)

# Send commands to VEX brain
ser.write(b"MOVE_FORWARD 100\n")
ser.write(b"TURN_LEFT 90\n")

# Read responses
while True:
    if ser.in_waiting:
        response = ser.readline().decode('utf-8').strip()
        print(f"VEX Response: {response}")
    time.sleep(0.1)
```

### Node.js Example
```javascript
const SerialPort = require('serialport');

const port = new SerialPort('/dev/ttyUSB0', {
    baudRate: 115200
});

port.on('data', (data) => {
    console.log('Received:', data.toString());
});

port.write('MOVE_FORWARD 100\n');
```

### Arduino IDE Serial Monitor
1. Open Arduino IDE
2. Select the correct port (VEX brain)
3. Set baud rate to 115200
4. Send commands in the serial monitor

## Integration with Robot Control

### Example: Command Parser
```cpp
void handleSerialCommand(const std::string& command) {
    std::istringstream iss(command);
    std::string action;
    iss >> action;
    
    if (action == "MOVE_FORWARD") {
        float distance;
        iss >> distance;
        chassis.drive_distance(distance);
    }
    else if (action == "TURN_LEFT") {
        float angle;
        iss >> angle;
        chassis.turn_to_heading(angle);
    }
    else if (action == "STOP") {
        chassis.stop(brake);
    }
    else if (action == "STATUS") {
        serial.send("Robot status: OK\n");
    }
}

// In main loop
serial.onMessage(handleSerialCommand);
```

### Example: Autonomous Mode Control
```cpp
void autonomous() {
    SerialCommunication serial;
    serial.connect();
    
    serial.onMessage([](const std::string& message) {
        if (message == "START_AUTON") {
            run_auton_item();
        }
        else if (message == "STOP_AUTON") {
            chassis.stop(brake);
        }
    });
    
    while (true) {
        serial.poll();
        wait(20, msec);
    }
}
```

## Troubleshooting

### Common Issues

1. **No Communication**: Ensure USB cable is properly connected
2. **Wrong Port**: Check device manager/port settings
3. **Baud Rate Mismatch**: Ensure both sides use same baud rate
4. **Buffer Overflow**: Call `poll()` regularly in main loop

### Debug Tips

1. **Echo Messages**: Send back received messages to verify communication
2. **Status Messages**: Send periodic status updates
3. **Error Handling**: Implement error callbacks for debugging

## Future Enhancements

1. **Binary Protocol**: Support for binary data transmission
2. **JSON Messages**: Structured message format
3. **Compression**: Data compression for large messages
4. **Encryption**: Secure communication
5. **Multiple Connections**: Support for multiple devices 