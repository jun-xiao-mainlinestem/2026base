import Foundation
import Network
import Speech
import AVFoundation
import CoreMotion
import UIKit // Added for UIDeviceOrientation

class RobotController: ObservableObject {
    @Published var isConnected = false
    @Published var recognizedText = ""
    @Published var isListening = false
    @Published var deviceOrientation: UIDeviceOrientation = .portrait
    @Published var absoluteTiltAngle: Double = 0.0 // Absolute pitch angle in degrees (forward/backward)
    @Published var absoluteRollAngle: Double = 0.0 // Absolute roll angle in degrees (left/right)
    
    private var webSocket: URLSessionWebSocketTask?
    private var speechRecognizer: SFSpeechRecognizer?
    private var recognitionRequest: SFSpeechAudioBufferRecognitionRequest?
    private var recognitionTask: SFSpeechRecognitionTask?
    private var audioEngine: AVAudioEngine?
    private let motionManager = CMMotionManager()
    
    init() {
        speechRecognizer = SFSpeechRecognizer(locale: Locale(identifier: "en-US"))
        setupSpeechRecognition()
        setupMotionManager()
    }
    
    // MARK: - WebSocket Communication
    
    func connect(deviceId: String, ipAddress: String) {
        // Trim whitespace from IP address to prevent invalid URLs
        let cleanIpAddress = ipAddress.trimmingCharacters(in: .whitespacesAndNewlines)
        
        // Ensure proper WebSocket URL format
        let urlString: String
        if cleanIpAddress == "localhost" {
            urlString = "ws://localhost:7071"
        } else {
            urlString = "ws://\(cleanIpAddress):7071"
        }
        
        print("RobotController: Attempting to connect to: \(urlString)")
        print("RobotController: Original IP: '\(ipAddress)', Cleaned IP: '\(cleanIpAddress)'")
        
        guard let url = URL(string: urlString) else {
            print("RobotController: Invalid WebSocket URL: \(urlString)")
            return
        }
        
        print("RobotController: URL created successfully: \(url)")
        print("RobotController: URL scheme: \(url.scheme ?? "nil")")
        print("RobotController: URL host: \(url.host ?? "nil")")
        print("RobotController: URL port: \(url.port ?? -1)")
        
        let session = URLSession(configuration: .default)
        webSocket = session.webSocketTask(with: url)
        webSocket?.resume()
        
        // Start listening for messages
        receiveMessage()
        
        isConnected = true
        print("RobotController: WebSocket connection initiated, isConnected set to true")
    }
    
    func disconnect() {
        webSocket?.cancel()
        webSocket = nil
        isConnected = false
    }
    
    func sendCommand(_ command: String) {
        guard isConnected else { return }
        
        let message = URLSessionWebSocketTask.Message.string(command)
        webSocket?.send(message) { error in
            if let error = error {
                print("WebSocket send error: \(error)")
            }
        }
    }
    
    private func receiveMessage() {
        webSocket?.receive { [weak self] result in
            switch result {
            case .success(let message):
                switch message {
                case .string(let text):
                    print("RobotController: Received message: \(text)")
                case .data(let data):
                    print("RobotController: Received data: \(data.count) bytes")
                @unknown default:
                    break
                }
                // Continue listening
                self?.receiveMessage()
                
            case .failure(let error):
                print("RobotController: WebSocket receive error: \(error)")
                DispatchQueue.main.async {
                    self?.isConnected = false
                }
            }
        }
    }
    
    // MARK: - Speech Recognition
    
    private func setupSpeechRecognition() {
        speechRecognizer = SFSpeechRecognizer(locale: Locale(identifier: "en-US"))
        
        // Request authorization
        SFSpeechRecognizer.requestAuthorization { authStatus in
            DispatchQueue.main.async {
                switch authStatus {
                case .authorized:
                    print("Speech recognition authorized")
                case .denied:
                    print("Speech recognition denied")
                case .restricted:
                    print("Speech recognition restricted")
                case .notDetermined:
                    print("Speech recognition not determined")
                @unknown default:
                    print("Speech recognition unknown status")
                }
            }
        }
    }
    
    func startSpeechRecognition() {
        guard let speechRecognizer = speechRecognizer, speechRecognizer.isAvailable else {
            print("Speech recognition not available")
            return
        }
        
        // Stop any existing recognition
        stopSpeechRecognition()
        
        // Configure audio session
        let audioSession = AVAudioSession.sharedInstance()
        do {
            try audioSession.setCategory(.record, mode: .measurement)
            try audioSession.setActive(true)
        } catch {
            print("Audio session setup failed: \(error)")
            return
        }
        
        // Create recognition request
        recognitionRequest = SFSpeechAudioBufferRecognitionRequest()
        guard let recognitionRequest = recognitionRequest else {
            print("Unable to create recognition request")
            return
        }
        recognitionRequest.shouldReportPartialResults = true
        
        // Create audio engine
        audioEngine = AVAudioEngine()
        guard let audioEngine = audioEngine else {
            print("Unable to create audio engine")
            return
        }
        
        let inputNode = audioEngine.inputNode
        let recordingFormat = inputNode.outputFormat(forBus: 0)
        inputNode.installTap(onBus: 0, bufferSize: 1024, format: recordingFormat) { buffer, _ in
            recognitionRequest.append(buffer)
        }
        
        // Start audio engine
        do {
            audioEngine.prepare()
            try audioEngine.start()
        } catch {
            print("Audio engine start failed: \(error)")
            return
        }
        
        // Start recognition
        recognitionTask = speechRecognizer.recognitionTask(with: recognitionRequest) { [weak self] result, error in
            if let result = result {
                let recognizedText = result.bestTranscription.formattedString
                DispatchQueue.main.async {
                    self?.recognizedText = recognizedText
                }
                
                // Send the complete recognized text when recognition is final
                if result.isFinal {
                    self?.sendCommand(recognizedText)
                }
            }
            
            if error != nil {
                self?.stopSpeechRecognition()
            }
        }
        
        isListening = true
    }
    
    func stopSpeechRecognition() {
        recognitionTask?.cancel()
        recognitionTask = nil
        
        audioEngine?.stop()
        audioEngine?.inputNode.removeTap(onBus: 0)
        audioEngine = nil
        
        recognitionRequest?.endAudio()
        recognitionRequest = nil
        
        isListening = false
    }
    
    // MARK: - Motion Manager
    
    private func setupMotionManager() {
        print("RobotController: Setting up motion manager...")
        
        guard motionManager.isDeviceMotionAvailable else {
            print("RobotController: Device motion is not available")
            return
        }
        
        print("RobotController: Device motion is available, starting updates...")
        motionManager.deviceMotionUpdateInterval = 0.1 // 10 Hz updates
        
        motionManager.startDeviceMotionUpdates(to: .main) { [weak self] motion, error in
            guard let motion = motion, error == nil else {
                print("RobotController: Device motion error: \(error?.localizedDescription ?? "unknown")")
                return
            }
            
            // Get absolute pitch angle (forward/backward tilt)
            let pitchDegrees = motion.attitude.pitch * 180.0 / .pi
            self?.absoluteTiltAngle = pitchDegrees
            
            // Get absolute roll angle (left/right tilt)
            let rollDegrees = motion.attitude.roll * 180.0 / .pi
            self?.absoluteRollAngle = rollDegrees
            
            print("RobotController: Device pitch: \(pitchDegrees)°, Roll: \(rollDegrees)°")
        }
        
        print("RobotController: Motion manager setup complete")
    }
    
    // MARK: - Permissions
    
    func requestPermissions() {
        // Speech recognition permission is requested in setupSpeechRecognition()
        // Motion permissions are automatically granted
        setupMotionManager()
    }
    
    deinit {
        stopSpeechRecognition()
        motionManager.stopDeviceMotionUpdates()
        disconnect()
    }
}
