//
//  ContentView.swift
//  remoteApp
//
//  Created by Jun Xiao on 8/15/25.
//

import SwiftUI
import Speech
import CoreMotion

struct ContentView: View {
    @StateObject private var robotController = RobotController()
    @State private var deviceId = ""
    @State private var ipAddress = ""
    @State private var isConnected = false
    
    var body: some View {
        Group {
            if !isConnected {
                // Connection Screen
                ConnectionScreen(
                    deviceId: $deviceId,
                    ipAddress: $ipAddress,
                    onConnect: {
                        print("Connect button tapped - attempting connection...")
                        robotController.connect(deviceId: deviceId, ipAddress: ipAddress)
                    }
                )
            } else {
                // Drive Screen
                DriveScreen(
                    robotController: robotController,
                    onDisconnect: {
                        print("Disconnect button tapped")
                        robotController.disconnect()
                    }
                )
            }
        }
        .onAppear {
            robotController.requestPermissions()
        }
        .onReceive(robotController.$isConnected) { connected in
            print("Connection state changed: \(connected)")
            isConnected = connected
        }
    }
}

// Connection Screen
struct ConnectionScreen: View {
    @Binding var deviceId: String
    @Binding var ipAddress: String
    let onConnect: () -> Void
    
    var body: some View {
        VStack(spacing: 30) {
            // Header
            VStack(spacing: 10) {
                Image(systemName: "robot")
                    .font(.system(size: 80))
                    .foregroundColor(.blue)
                
                Text("VEX Robot Control")
                    .font(.largeTitle)
                    .fontWeight(.bold)
                
                Text("Connect to your robot to get started")
                    .font(.title3)
                    .foregroundColor(.secondary)
            }
            
            // Connection Form
            VStack(spacing: 20) {
                VStack(alignment: .leading, spacing: 8) {
                    Text("IP Address")
                        .font(.headline)
                        .foregroundColor(.primary)
                    
                    TextField("Enter robot IP address", text: $ipAddress)
                        .textFieldStyle(RoundedBorderTextFieldStyle())
                        .keyboardType(.decimalPad)
                        .autocapitalization(.none)
                        .disableAutocorrection(true)
                }
                
                VStack(alignment: .leading, spacing: 8) {
                    Text("Device ID")
                        .font(.headline)
                        .foregroundColor(.primary)
                    
                    TextField("Enter device ID", text: $deviceId)
                        .textFieldStyle(RoundedBorderTextFieldStyle())
                        .keyboardType(.numberPad)
                }
                
                Button(action: onConnect) {
                    HStack {
                        Image(systemName: "wifi")
                        Text("Connect")
                    }
                    .frame(maxWidth: .infinity)
                    .padding()
                    .background(Color.blue)
                    .foregroundColor(.white)
                    .cornerRadius(10)
                }
                .disabled(deviceId.isEmpty || ipAddress.isEmpty)
                .padding(.horizontal)
            }
            .padding()
            .background(Color(.systemGray6))
            .cornerRadius(15)
            .padding(.horizontal)
            .toolbar {
                ToolbarItemGroup(placement: .keyboard) {
                    Spacer()
                    Button("Done") {
                        UIApplication.shared.sendAction(#selector(UIResponder.resignFirstResponder), to: nil, from: nil, for: nil)
                    }
                }
            }
            
            Spacer()
        }
        .padding()
        .background(Color(.systemBackground))
    }
}

// Drive Screen
struct DriveScreen: View {
    @ObservedObject var robotController: RobotController
    let onDisconnect: () -> Void
    
    @State private var recognizedText = ""
    @State private var isListening = false
    @State private var isDriving = false
    @State private var leftVoltage: Double = 0
    @State private var rightVoltage: Double = 0
    @State private var lastCommand: String = ""
    
    // Separate tracking for pitch (forward/backward)
    @State private var pitchStableCount: Int = 0
    @State private var lastStablePitchVoltage: Double = 0
    @State private var lastSentPitchVoltage: Double = 0
    
    // Separate tracking for roll (left/right turning)
    @State private var rollStableCount: Int = 0
    @State private var lastStableRollVoltage: Double = 0
    @State private var lastSentRollVoltage: Double = 0
    
    var body: some View {
        VStack(spacing: 30) {
            // Top - Speech Control
            VStack(spacing: 20) {
                Text("Speech Control")
                    .font(.title2)
                    .fontWeight(.bold)
                
                Button(action: {
                    if isListening {
                        robotController.stopSpeechRecognition()
                    } else {
                        robotController.startSpeechRecognition()
                    }
                }) {
                    VStack(spacing: 10) {
                        Image(systemName: isListening ? "mic.fill" : "mic")
                            .font(.system(size: 40))
                            .foregroundColor(isListening ? .red : .blue)
                        Text(isListening ? "Stop Listening" : "Start Listening")
                            .font(.headline)
                    }
                    .frame(width: 200, height: 120)
                    .padding()
                    .background(isListening ? Color.red.opacity(0.2) : Color.blue.opacity(0.2))
                    .foregroundColor(isListening ? .red : .blue)
                    .cornerRadius(15)
                }
                
                if !recognizedText.isEmpty {
                    Text("Recognized: \(recognizedText)")
                        .font(.caption)
                        .padding()
                        .background(Color(.systemGray5))
                        .cornerRadius(8)
                }
            }
            
            Spacer()
            
            // Bottom - Drive Control
            VStack(spacing: 20) {
                Text("Drive Control")
                    .font(.title2)
                    .fontWeight(.bold)
                
                Button(action: {
                    isDriving.toggle()
                    if isDriving {
                        // Start driving - send vol 0 0 to establish neutral position
                        leftVoltage = 0
                        rightVoltage = 0
                        lastSentPitchVoltage = 0
                        lastStablePitchVoltage = 0
                        pitchStableCount = 0
                        lastSentRollVoltage = 0
                        lastStableRollVoltage = 0
                        rollStableCount = 0
                        updateDriveCommand()
                    } else {
                        // Stop the robot
                        leftVoltage = 0
                        rightVoltage = 0
                        lastSentPitchVoltage = 0
                        lastStablePitchVoltage = 0
                        pitchStableCount = 0
                        lastSentRollVoltage = 0
                        lastStableRollVoltage = 0
                        rollStableCount = 0
                        updateDriveCommand()
                    }
                }) {
                    VStack(spacing: 10) {
                        Image(systemName: isDriving ? "stop.fill" : "play.fill")
                            .font(.system(size: 40))
                            .foregroundColor(isDriving ? .red : .green)
                        Text(isDriving ? "Stop" : "Drive")
                            .font(.headline)
                    }
                    .frame(width: 200, height: 120)
                    .padding()
                    .background(isDriving ? Color.red.opacity(0.2) : Color.green.opacity(0.2))
                    .foregroundColor(isDriving ? .red : .green)
                    .cornerRadius(15)
                }
            }
            
            // WebSocket Command Display at Bottom
            VStack(spacing: 8) {
                Divider()
                if !lastCommand.isEmpty {
                    HStack {
                        Text("Last Sent:")
                            .font(.subheadline)
                            .foregroundColor(.secondary)
                        Text(lastCommand)
                            .font(.system(.subheadline, design: .monospaced))
                            .foregroundColor(.primary)
                            .padding(.horizontal, 8)
                            .padding(.vertical, 4)
                            .background(Color(.systemGray5))
                            .cornerRadius(6)
                        Spacer()
                    }
                }
            }
            .padding()
            .background(Color(.systemGray6))
            .cornerRadius(10)
            .padding(.horizontal)
        }
        .padding()
        .onReceive(robotController.$recognizedText) { text in
            recognizedText = text
        }
        .onReceive(robotController.$isListening) { listening in
            isListening = listening
        }
        .onReceive(robotController.$absoluteTiltAngle) { pitchAngle in
            // Only process tilt data when driving
            guard isDriving else { return }
            
            // Convert absolute pitch angle to voltage for forward/backward
            let newPitchVoltage = convertTiltToVoltage(pitchAngle)
            
            // Check if this voltage is stable (close to last stable voltage)
            let voltageDifference = abs(newPitchVoltage - lastStablePitchVoltage)
            let isStable = voltageDifference < 0.5
            
            if isStable {
                pitchStableCount += 1
            } else {
                pitchStableCount = 0
                lastStablePitchVoltage = newPitchVoltage
            }
            
            // Only send command if voltage has been stable for 3 readings
            let shouldSendCommand = pitchStableCount >= 3
            
            if shouldSendCommand {
                // Update pitch voltage and recalculate combined voltages
                lastSentPitchVoltage = newPitchVoltage
                updateCombinedVoltages()
                
                print("Pitch: \(pitchAngle)°, voltage: \(newPitchVoltage)V, stable count: \(pitchStableCount)")
            }
        }
        .onReceive(robotController.$absoluteRollAngle) { rollAngle in
            // Only process roll data when driving
            guard isDriving else { return }
            
            // Convert absolute roll angle to voltage for left/right turning
            let newRollVoltage = convertTiltToVoltage(rollAngle)
            
            // Check if this voltage is stable (close to last stable voltage)
            let voltageDifference = abs(newRollVoltage - lastStableRollVoltage)
            let isStable = voltageDifference < 0.5
            
            if isStable {
                rollStableCount += 1
            } else {
                rollStableCount = 0
                lastStableRollVoltage = newRollVoltage
            }
            
            // Only send command if voltage has been stable for 3 readings
            let shouldSendCommand = rollStableCount >= 3
            
            if shouldSendCommand {
                // Update roll voltage and recalculate combined voltages
                lastSentRollVoltage = newRollVoltage
                updateCombinedVoltages()
                
                print("Roll: \(rollAngle)°, voltage: \(newRollVoltage)V, stable count: \(rollStableCount)")
            }
        }
    }
    
    private func updateDriveCommand() {
        let command = "vol \(Int(leftVoltage)) \(Int(rightVoltage))"
        robotController.sendCommand(command)
        lastCommand = command
    }
    
    // Update combined voltages using additive mapping
    private func updateCombinedVoltages() {
        // Base voltage is 0V when flat
        // Forward/backward tilt adds/subtracts to both motors equally
        // Left/right tilt adds to one motor and subtracts from the other
        
        let pitchVoltage = lastSentPitchVoltage  // Forward/backward
        let rollVoltage = lastSentRollVoltage    // Left/right
        
        // Calculate left and right motor voltages
        leftVoltage = pitchVoltage + rollVoltage   // Left motor: forward + left
        rightVoltage = pitchVoltage - rollVoltage  // Right motor: forward - left
        
        // Clamp voltages to maximum ±12V (6V from pitch + 6V from roll)
        leftVoltage = max(-12.0, min(12.0, leftVoltage))
        rightVoltage = max(-12.0, min(12.0, rightVoltage))
        
        // Send the combined command
        updateDriveCommand()
        
        print("Combined: Pitch=\(pitchVoltage)V, Roll=\(rollVoltage)V → Left=\(leftVoltage)V, Right=\(rightVoltage)V")
    }
    
    // Convert absolute tilt angle to voltage levels
    private func convertTiltToVoltage(_ tiltAngle: Double) -> Double {
        // tiltAngle is already in degrees from device motion
        
        // Much more sensitive voltage mapping based on absolute tilt angle
        if abs(tiltAngle) < 8.0 {
            // Small dead zone: very small tilts (0-8 degrees) = no movement (0V)
            return 0.0
        } else if abs(tiltAngle) < 30.0 {
            // Linear mapping from 8° to 30°: 0V to ±6V
            let normalizedTilt = (abs(tiltAngle) - 8.0) / 22.0  // 0.0 to 1.0
            let voltage = normalizedTilt * 6.0  // 0V to 6V
            return tiltAngle > 0 ? voltage : -voltage
        } else {
            // Maximum tilt (> 30 degrees) = full speed (±6V)
            return tiltAngle > 0 ? 6.0 : -6.0
        }
    }
}

#Preview {
    ContentView()
}
