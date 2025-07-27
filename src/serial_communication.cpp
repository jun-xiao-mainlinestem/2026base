#include "serial_communication.h"
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
    send("VEX_BRAIN_CONNECTED\n");
    
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
    
    // Write to the user port (stdio) - this sends data back to the computer
    // This allows the computer to receive responses from the VEX brain
    printf("%s", message.c_str());
    
    // Also show on controller screen for debugging
    controller(primary).Screen.clearScreen();
    controller(primary).Screen.print("Sent: %s", message.c_str());
    
    return true;
}

void SerialCommunication::poll() {
    if (!connected) return;
    
    // For now, we'll use a simple simulation to test the command processing
    // In a real implementation, this would read from the user port
    static int pollCounter = 0;
    pollCounter++;
    
    // Simulate receiving a command every 100 polls (about every 10 seconds)
    if (pollCounter % 100 == 0) {
        // This simulates what would come from the user port
        std::string simulatedMessage = "FORWARD\n";
        buffer += simulatedMessage;
        processBuffer();
        
        // Also simulate a STOP command after 2 seconds
        if (pollCounter % 200 == 0) {
            std::string stopMessage = "STOP\n";
            buffer += stopMessage;
            processBuffer();
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