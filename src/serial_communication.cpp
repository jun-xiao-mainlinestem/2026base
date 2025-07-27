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
    // Open serial port if not already open
    if (!serialFile) {
        serialFile = fopen("/dev/serial1", "rb");
        if (!serialFile) {
            controller(primary).Screen.clearScreen();
            controller(primary).Screen.print("can't open serial");
            controller(primary).rumble(".");
            return;
        }
    }
    // Read one byte at a time
    char c;
    if (fread(&c, 1, 1, serialFile) == 1) {
        // Map character to command and act
        if (c == 'a' || c == 'A') {
            controller(primary).Screen.clearScreen();
            controller(primary).Screen.print("FORWARD");
            controller(primary).rumble(".");
            chassis.drive_with_voltage(2, 2);
        } else if (c == 'p' || c == 'P') {
            controller(primary).Screen.clearScreen();
            controller(primary).Screen.print("STOP");
            controller(primary).rumble(".");
            float current_heading = chassis.get_heading();
            float distance_traveled = (chassis.get_left_position_in() + chassis.get_right_position_in()) / 2.0;
            chassis.stop(brake);
            stop_rollers();
            
            // Send status back to webpage            
            char status_message[100];
            sprintf(status_message, "STATUS:%.1f:%.1f\n", current_heading, distance_traveled);
            send(status_message);

        } else if (c == 'd' || c == 'D') {
            controller(primary).Screen.clearScreen();
            controller(primary).Screen.print("RIGHT");
            controller(primary).rumble(".");
            chassis.drive_with_voltage(2, -2);
        } else if (c == 'l' || c == 'L') {
            controller(primary).Screen.clearScreen();
            controller(primary).Screen.print("LEFT");
            controller(primary).rumble(".");
            chassis.drive_with_voltage(-2, 2);
        } else if (c == 'b' || c == 'B') {
            controller(primary).Screen.clearScreen();
            controller(primary).Screen.print("BACKWARD");
            controller(primary).rumble(".");
            chassis.drive_with_voltage(-2, -2);
        } else if (c == 'i' || c == 'I') {
            controller(primary).Screen.clearScreen();
            controller(primary).Screen.print("ROLL");
            controller(primary).rumble(".");
            in_take();
        } else if (c == 's' || c == 'S') {
            controller(primary).Screen.clearScreen();
            controller(primary).Screen.print("SHOOT");
            controller(primary).rumble(".");
            score_long();
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