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
    
public:
    RemoteControl();
    ~RemoteControl();
    
    // Connection methods
    bool connect();
    void disconnect();
    bool isConnected() const { return connected; }
    
    // Communication methods
    bool send(const std::string& message);
    void poll();
};

} // namespace rgb 