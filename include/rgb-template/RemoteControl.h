#pragma once

#include "vex.h"
#include <string>

namespace rgb {

class RemoteControl {
private:
    bool connected;
    static FILE* serialFile;
    static std::string lineBuffer;
    
    void processCommand(const std::string& command);
    void executeCommand(const std::string& cmd);
    void updateStatus(bool success, const char* status_message);
    
public:
    RemoteControl();
    ~RemoteControl();
    
    // Connection methods
    bool connect();
    void disconnect();
    bool isConnected() const { return connected; }
    bool attemptConnection();
    
    // Communication methods
    bool send(const std::string& message);
    void poll();
};

} // namespace rgb 