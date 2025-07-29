#include "serial_communication.h"
#include "chassis.h"
#include <sstream>
#include <cstring>

namespace serial {

SerialCommunication::SerialCommunication() : connected(false) {
    // Initialize serial communication
    // The VEX brain uses the USB port for serial communication
}

SerialCommunication::~SerialCommunication() {
    if (connected) {
        disconnect();
    }
}

bool SerialCommunication::connect() {
    // For VEX V5, the USB port is automatically available for serial communication
    // We just need to check if it's working
    connected = true;
    
    // Send a test message to verify connection
 //   send("VEX_BRAIN_CONNECTED\n");
    
    return connected;
}

void SerialCommunication::disconnect() {
    if (connected) {
        send("VEX_BRAIN_DISCONNECTED\n");
        connected = false;
    }
}

bool SerialCommunication::send(const std::string& message) {
    if (!connected) return false;
    
    // For responses, we can use printf to send data back to the computer
    // This will appear in the debug terminal
    printf("%s", message.c_str());
    
    // Also show on controller screen for debugging
    controller(primary).Screen.clearScreen();
    controller(primary).Screen.print("Sent: %s", message.c_str());
    
    return true;
}

void SerialCommunication::poll() {
    if (!connected) return;
    static FILE* serialFile = nullptr;
    static std::string lineBuffer = "";
    static int totalBytesRead = 0;

    if (!serialFile) {
        // Use /dev/serial1 as it was working before
        serialFile = fopen("/dev/serial1", "r");
        if (serialFile) {
            controller(primary).Screen.clearScreen();
            controller(primary).rumble(".");
        } else {
            controller(primary).Screen.clearScreen();
            controller(primary).rumble("---");
            return;
        }
    }


    char readBuf[128];
    size_t bytesRead = fread(readBuf, 1, sizeof(readBuf), serialFile);
    totalBytesRead += bytesRead;
    
    if (bytesRead > 0) {
        // Print raw buffer to controller for debugging
        controller(primary).Screen.clearScreen();
        controller(primary).Screen.print("Bytes read: %d", (int)bytesRead);
        
        for (size_t i = 0; i < bytesRead; ++i) {
            char c = readBuf[i];
            controller(primary).Screen.newLine();
            controller(primary).Screen.print("[%d] 0x%02X '%c'", (int)i, (unsigned char)c, (c >= 32 && c <= 126) ? c : '.');
            
            // Process each character immediately (for single char commands)
            if (c == '\n' || c == '\r') {
                if (!lineBuffer.empty()) {
                    controller(primary).Screen.newLine();
                    controller(primary).Screen.print("Command: %s", lineBuffer.c_str());
                    processCommand(lineBuffer);
                    lineBuffer.clear();
                }
            } else {
                lineBuffer += c;
            }
        }
    }
}

void SerialCommunication::processCommand(const std::string& command) {
    // Handle both single characters and full commands
    std::string cmd = command;
    
    // Remove any whitespace and newlines
    cmd.erase(0, cmd.find_first_not_of(" \t\r\n"));
    cmd.erase(cmd.find_last_not_of(" \t\r\n") + 1);
    
    // If it's a single character, map it directly
    if (cmd.length() == 1) {
        char c = cmd[0];
        switch (c) {
            case 'w': case 'W': // Forward
                controller(primary).Screen.clearScreen();
                controller(primary).Screen.print("Command: FORWARD (w)");
                chassis.drive_with_voltage(3, 3);
                controller(primary).rumble(".");
                break;
            case 's': case 'S': // Backward
                controller(primary).Screen.clearScreen();
                controller(primary).Screen.print("Command: BACKWARD (s)");
                chassis.drive_with_voltage(-3, -3);
                controller(primary).rumble(".");
                break;
            case 'a': case 'A': // Left
                controller(primary).Screen.clearScreen();
                controller(primary).Screen.print("Command: LEFT (a)");
                chassis.drive_with_voltage(-3, 3);
                controller(primary).rumble(".");
                break;
            case 'd': case 'D': // Right
                controller(primary).Screen.clearScreen();
                controller(primary).Screen.print("Command: RIGHT (d)");
                chassis.drive_with_voltage(3, -3);
                controller(primary).rumble(".");
                break;
            case 'x': case 'X': // Stop
                controller(primary).Screen.clearScreen();
                controller(primary).Screen.print("Command: STOP (x)");
                chassis.drive_with_voltage(0, 0);
                controller(primary).rumble("---");
                
                // Send status back
                char statusMsg[128];
                sprintf(statusMsg, "STATUS:HEADING=%.2f\n", 
                        chassis.get_heading());
                send(statusMsg);
                break;
            case 'r': case 'R': // Roll/Intake
                controller(primary).Screen.clearScreen();
                controller(primary).Screen.print("Command: ROLL (r)");
                in_take();
                controller(primary).rumble(".");
                break;
            case 'f': case 'F': // Shoot/Score
                controller(primary).Screen.clearScreen();
                controller(primary).Screen.print("Command: SHOOT (f)");
                score_long();
                controller(primary).rumble(".");
                break;
            default:
                controller(primary).Screen.clearScreen();
                controller(primary).Screen.print("UNKNOWN CHAR: %c", c);
                controller(primary).rumble(".");
                break;
        }
    } else {
        // Handle full text commands (convert to uppercase)
        for (char& c : cmd) {
            c = toupper(c);
        }
        
        if (cmd == "FORWARD" || cmd == "MOVE" || cmd == "GO") {
            controller(primary).Screen.clearScreen();
            controller(primary).Screen.print("Command: FORWARD");
            chassis.drive_with_voltage(3, 3);
            controller(primary).rumble(".");
        }
        else if (cmd == "BACKWARD" || cmd == "BACK") {
            controller(primary).Screen.clearScreen();
            controller(primary).Screen.print("Command: BACKWARD");
            chassis.drive_with_voltage(-3, -3);
            controller(primary).rumble(".");
        }
        else if (cmd == "LEFT") {
            controller(primary).Screen.clearScreen();
            controller(primary).Screen.print("Command: LEFT");
            chassis.drive_with_voltage(-3, 3);
            controller(primary).rumble(".");
        }
        else if (cmd == "RIGHT") {
            controller(primary).Screen.clearScreen();
            controller(primary).Screen.print("Command: RIGHT");
            chassis.drive_with_voltage(3, -3);
            controller(primary).rumble(".");
        }
        else if (cmd == "STOP") {
            controller(primary).Screen.clearScreen();
            controller(primary).Screen.print("Command: STOP");
            chassis.drive_with_voltage(0, 0);
            controller(primary).rumble("---");
            
            // Send status back
            char statusMsg[128];
            sprintf(statusMsg, "STATUS:HEADING=%.2f\n", 
                    chassis.get_heading());
            send(statusMsg);
        }
        else if (cmd == "ROLL" || cmd == "INTAKE") {
            controller(primary).Screen.clearScreen();
            controller(primary).Screen.print("Command: ROLL");
            in_take();
            controller(primary).rumble(".");
        }
        else if (cmd == "SHOOT" || cmd == "SCORE") {
            controller(primary).Screen.clearScreen();
            controller(primary).Screen.print("Command: SHOOT");
            score_long();
            controller(primary).rumble(".");
        }
        else {
            controller(primary).Screen.clearScreen();
            controller(primary).Screen.print("UNKNOWN: %s", command.c_str());
            controller(primary).rumble(".");
        }
    }
}

std::string SerialCommunication::readLine() {
    if (buffer.empty()) return "";
    
    size_t newlinePos = buffer.find('\n');
    if (newlinePos == std::string::npos) return "";
    
    std::string line = buffer.substr(0, newlinePos);
    buffer = buffer.substr(newlinePos + 1);
    
    return line;
}

std::string SerialCommunication::readAll() {
    std::string data = buffer;
    buffer.clear();
    return data;
}

void SerialCommunication::processBuffer() {
    std::string message = extractMessage();
    if (!message.empty() && messageCallback) {
        messageCallback(message);
    }
}

std::string SerialCommunication::extractMessage() {
    size_t newlinePos = buffer.find('\n');
    if (newlinePos == std::string::npos) return "";
    
    std::string message = buffer.substr(0, newlinePos);
    buffer = buffer.substr(newlinePos + 1);
    
    // Remove carriage return if present
    if (!message.empty() && message.back() == '\r') {
        message.pop_back();
    }
    
    return message;
}

void SerialCommunication::clearBuffer() {
    buffer.clear();
}

bool SerialCommunication::hasData() const {
    return !buffer.empty();
}

int SerialCommunication::availableBytes() const {
    return static_cast<int>(buffer.length());
}

} // namespace serial 