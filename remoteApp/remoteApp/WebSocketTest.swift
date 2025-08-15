import Foundation
import Network

class WebSocketTest: ObservableObject {
    @Published var connectionStatus = "Disconnected"
    @Published var lastMessage = ""
    @Published var isConnected = false
    
    private var webSocket: URLSessionWebSocketTask?
    
    func testConnection(deviceId: String, host: String = "localhost", port: Int = 7071) {
        let urlString = "ws://\(host):\(port)"
        
        guard let url = URL(string: urlString) else {
            connectionStatus = "Invalid URL"
            return
        }
        
        connectionStatus = "Connecting..."
        
        let session = URLSession(configuration: .default)
        webSocket = session.webSocketTask(with: url)
        webSocket?.resume()
        
        // Start listening for messages
        receiveMessage()
        
        // Send a test message
        DispatchQueue.main.asyncAfter(deadline: .now() + 1.0) {
            self.sendTestMessage()
        }
    }
    
    private func receiveMessage() {
        webSocket?.receive { [weak self] result in
            DispatchQueue.main.async {
                switch result {
                case .success(let message):
                    switch message {
                    case .string(let text):
                        self?.lastMessage = "Received: \(text)"
                        self?.connectionStatus = "Connected - Message received"
                        self?.isConnected = true
                    case .data(let data):
                        self?.lastMessage = "Received data: \(data.count) bytes"
                        self?.connectionStatus = "Connected - Data received"
                        self?.isConnected = true
                    @unknown default:
                        break
                    }
                    // Continue listening
                    self?.receiveMessage()
                    
                case .failure(let error):
                    self?.connectionStatus = "Error: \(error.localizedDescription)"
                    self?.isConnected = false
                }
            }
        }
    }
    
    private func sendTestMessage() {
        let testMessage = "test connection"
        webSocket?.send(.string(testMessage)) { error in
            DispatchQueue.main.async {
                if let error = error {
                    self.connectionStatus = "Send error: \(error.localizedDescription)"
                } else {
                    self.connectionStatus = "Connected - Test message sent"
                    self.isConnected = true
                }
            }
        }
    }
    
    func disconnect() {
        webSocket?.cancel()
        webSocket = nil
        connectionStatus = "Disconnected"
        isConnected = false
    }
    
    func sendCommand(_ command: String) {
        guard isConnected else {
            connectionStatus = "Not connected"
            return
        }
        
        webSocket?.send(.string(command)) { error in
            DispatchQueue.main.async {
                if let error = error {
                    self.connectionStatus = "Send error: \(error.localizedDescription)"
                } else {
                    self.lastMessage = "Sent: \(command)"
                }
            }
        }
    }
}
