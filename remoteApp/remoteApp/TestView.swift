import SwiftUI

struct TestView: View {
    @StateObject private var webSocketTest = WebSocketTest()
    @State private var deviceId = ""
    @State private var ipAddress = ""
    @State private var testCommand = ""
    
    var body: some View {
        VStack(spacing: 20) {
            Text("WebSocket Connection Test")
                .font(.title)
                .fontWeight(.bold)
            
            // Connection Section
            VStack(alignment: .leading, spacing: 10) {
                Text("Test Connection")
                    .font(.headline)
                
                VStack(spacing: 10) {
                    HStack {
                        Text("IP Address:")
                            .font(.subheadline)
                            .frame(width: 80, alignment: .leading)
                        
                        TextField("IP Address", text: $ipAddress)
                            .textFieldStyle(RoundedBorderTextFieldStyle())
                            .keyboardType(.URL)
                            .autocapitalization(.none)
                            .disableAutocorrection(true)
                    }
                    
                    HStack {
                        Text("Device ID:")
                            .font(.subheadline)
                            .frame(width: 80, alignment: .leading)
                        
                        TextField("Device ID", text: $deviceId)
                            .textFieldStyle(RoundedBorderTextFieldStyle())
                            .keyboardType(.numberPad)
                    }
                    
                    Button("Test") {
                        webSocketTest.testConnection(deviceId: deviceId, host: ipAddress)
                    }
                    .disabled(deviceId.isEmpty || ipAddress.isEmpty)
                    .frame(maxWidth: .infinity)
                }
                
                if webSocketTest.isConnected {
                    Button("Disconnect") {
                        webSocketTest.disconnect()
                    }
                    .buttonStyle(.bordered)
                }
            }
            .padding()
            .background(Color(.systemGray6))
            .cornerRadius(10)
            
            // Status Section
            VStack(alignment: .leading, spacing: 10) {
                Text("Connection Status")
                    .font(.headline)
                
                Text(webSocketTest.connectionStatus)
                    .padding()
                    .background(Color(.systemGray5))
                    .cornerRadius(8)
                
                if !webSocketTest.lastMessage.isEmpty {
                    Text("Last Message:")
                        .font(.subheadline)
                        .fontWeight(.semibold)
                    
                    Text(webSocketTest.lastMessage)
                        .font(.caption)
                        .padding()
                        .background(Color(.systemGray5))
                        .cornerRadius(8)
                }
            }
            .padding()
            .background(Color(.systemGray6))
            .cornerRadius(10)
            
            // Test Commands Section
            VStack(alignment: .leading, spacing: 10) {
                Text("Test Commands")
                    .font(.headline)
                
                HStack {
                    TextField("Command", text: $testCommand)
                        .textFieldStyle(RoundedBorderTextFieldStyle())
                        .placeholder(when: testCommand.isEmpty) {
                            Text("e.g., vol 0.3 0.3").foregroundColor(.secondary)
                        }
                    
                    Button("Send") {
                        webSocketTest.sendCommand(testCommand)
                        testCommand = ""
                    }
                    .buttonStyle(.bordered)
                    .disabled(testCommand.isEmpty || !webSocketTest.isConnected)
                }
                
                // Quick test buttons
                HStack {
                    Button("Stop") {
                        webSocketTest.sendCommand("vol 0 0")
                    }
                    .buttonStyle(.bordered)
                    .disabled(!webSocketTest.isConnected)
                    
                    Button("Forward") {
                        webSocketTest.sendCommand("vol 0.3 0.3")
                    }
                    .buttonStyle(.bordered)
                    .disabled(!webSocketTest.isConnected)
                    
                    Button("Turn Left") {
                        webSocketTest.sendCommand("vol 0.2 0.4")
                    }
                    .buttonStyle(.bordered)
                    .disabled(!webSocketTest.isConnected)
                    
                    Button("Turn Right") {
                        webSocketTest.sendCommand("vol 0.4 0.2")
                    }
                    .buttonStyle(.bordered)
                    .disabled(!webSocketTest.isConnected)
                }
            }
            .padding()
            .background(Color(.systemGray6))
            .cornerRadius(10)
            
            Spacer()
        }
        .padding()
    }
}

// Extension for placeholder text
extension View {
    func placeholder<Content: View>(
        when shouldShow: Bool,
        alignment: Alignment = .leading,
        @ViewBuilder placeholder: () -> Content) -> some View {
        
        ZStack(alignment: alignment) {
            placeholder().opacity(shouldShow ? 1 : 0)
            self
        }
    }
}

#Preview {
    TestView()
}
