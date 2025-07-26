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
    
    // Use VEX's built-in serial communication
    // This will send data through the USB port
    printf("%s", message.c_str());
    
    return true;
}

void SerialCommunication::poll() {
    if (!connected) return;
    
    // Check for incoming data
    // In VEX V5, we can use scanf or similar functions to read from USB
    char ch;
    while (scanf("%c", &ch) == 1) {
        buffer += ch;
        
        // If we get a newline, process the message
        if (ch == '\n') {
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