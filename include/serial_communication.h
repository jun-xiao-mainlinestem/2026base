#pragma once

#include "vex.h"
#include <string>
#include <functional>
#include <vector>

namespace serial {

class SerialCommunication {
private:
    std::string buffer;
    std::function<void(const std::string&)> messageCallback;
    std::function<void(const std::string&)> errorCallback;
    bool connected;
    
    // Internal methods
    void processBuffer();
    std::string extractMessage();
    
public:
    SerialCommunication();
    ~SerialCommunication();
    
    // Connection methods
    bool connect();
    void disconnect();
    bool isConnected() const { return connected; }
    
    // Communication methods
    bool send(const std::string& message);
    void poll();
    std::string readLine();
    std::string readAll();
    
    // Callback setters
    void onMessage(std::function<void(const std::string&)> callback) {
        messageCallback = callback;
    }
    
    void onError(std::function<void(const std::string&)> callback) {
        errorCallback = callback;
    }
    
    // Utility methods
    void clearBuffer();
    bool hasData() const;
    int availableBytes() const;
};

} // namespace serial 