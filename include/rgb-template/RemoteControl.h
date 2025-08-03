#pragma once

#include "vex.h"
#include <string>

namespace rgb {

class RemoteControl {
private:
    static FILE* serialFile;
    static std::string lineBuffer;
    
    void processCommand(const std::string& command);
    
public:
    RemoteControl();
    ~RemoteControl();
    
    // Communication methods
    bool send(const std::string& message);
    void poll();
};

} // namespace rgb 