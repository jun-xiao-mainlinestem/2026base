import './styles.css';
import { VEXWebSocket } from './websocket';
import { WebSocketConfig, ParsedCommand } from './types';

class CommandLineInterface {
  private websocket: VEXWebSocket | null = null;
  private outputLog: HTMLElement;
  private commandInput: HTMLInputElement;
  private sendButton: HTMLButtonElement;
  private connectButton: HTMLButtonElement;
  private deviceIdInput: HTMLInputElement;
  private statusIndicator: HTMLElement;
  private statusText: HTMLElement;
  private resetButton: HTMLButtonElement;
  private downloadButton: HTMLButtonElement;
  private commandHistory: string[] = [];

  constructor() {
    this.outputLog = document.getElementById('output-log') as HTMLElement;
    this.commandInput = document.getElementById('command-input') as HTMLInputElement;
    this.sendButton = document.getElementById('send-btn') as HTMLButtonElement;
    this.connectButton = document.getElementById('connect-btn') as HTMLButtonElement;
    this.deviceIdInput = document.getElementById('device-id') as HTMLInputElement;
    this.statusIndicator = document.getElementById('status-indicator') as HTMLElement;
    this.statusText = document.getElementById('status-text') as HTMLElement;
    this.resetButton = document.getElementById('reset-btn') as HTMLButtonElement;
    this.downloadButton = document.getElementById('download-btn') as HTMLButtonElement;

    this.initializeEventListeners();
  }

  private initializeEventListeners(): void {
    // Connect button
    this.connectButton.addEventListener('click', () => {
      const deviceId = this.deviceIdInput.value.trim();
      if (deviceId) {
        this.connectToRobot(deviceId);
      } else {
        this.logError('Please enter a Device ID');
      }
    });

    // Send button
    this.sendButton.addEventListener('click', () => {
      this.sendCommand();
    });

    // Command input (Enter key)
    this.commandInput.addEventListener('keypress', (e) => {
      if (e.key === 'Enter') {
        this.sendCommand();
      }
    });

    // Device ID input (Enter key)
    this.deviceIdInput.addEventListener('keypress', (e) => {
      if (e.key === 'Enter') {
        this.connectButton.click();
      }
    });

    // Reset button
    this.resetButton.addEventListener('click', () => {
      this.resetHistory();
    });

    // Download button
    this.downloadButton.addEventListener('click', () => {
      this.downloadAsCpp();
    });
  }

  private async connectToRobot(deviceId: string): Promise<void> {
    this.logStatus(`Connecting to robot (Device ID: ${deviceId})...`);
    
    const config: WebSocketConfig = {
      id: deviceId,
      port: '7071',
      ip: '192.168.86.21'
    };

    this.websocket = new VEXWebSocket(config);
    
    // Set up WebSocket callbacks
    this.websocket.onMessage((data) => {
      this.handleRobotResponse(data);
    });

    this.websocket.onStatusChange((connected) => {
      this.updateConnectionStatus(connected);
      if (connected) {
        this.commandInput.disabled = false;
        this.sendButton.disabled = false;
        this.logStatus('Connected! You can now send commands.');
      } else {
        this.commandInput.disabled = true;
        this.sendButton.disabled = true;
      }
    });

    try {
      await this.websocket.connect();
    } catch (error) {
      this.logError(`Connection failed: ${error}`);
    }
  }

  private sendCommand(): void {
    const command = this.commandInput.value.trim();
    if (!command) return;

    this.logCommand(command);
    this.commandInput.value = '';

    const parsedCommand = this.parseCommand(command);
    if (parsedCommand) {
      const message = this.formatRobotCommand(parsedCommand);
      const apiCall = this.formatChassisAPICall(parsedCommand);
      
      // Store the API call for download
      this.commandHistory.push(apiCall);
      
      if (this.websocket && this.websocket.isConnected()) {
        this.websocket.send(message);
        this.logStatus(`Executing: ${apiCall}`);
      } else {
        this.logError('Failed to send command - not connected');
      }
    } else {
      this.logError('Invalid command format');
    }
  }

  private parseCommand(command: string): ParsedCommand | null {
    const parts = command.toLowerCase().split(' ');
    const action = parts[0];

    switch (action) {
      case 'drive':
        return this.parseDriveCommand(parts);
      case 'turn':
        return this.parseTurnCommand(parts);
      default:
        return null;
    }
  }

  private parseDriveCommand(parts: string[]): ParsedCommand | null {
    if (parts.length < 2) {
      this.logError('Drive command requires distance parameter');
      return null;
    }

    const distance = parseFloat(parts[1]);
    if (isNaN(distance)) {
      this.logError('Invalid distance value');
      return null;
    }

    const command: ParsedCommand = {
      type: 'drive',
      distance: distance,
      voltage: 10 // default
    };

    // Optional voltage parameter
    if (parts.length >= 3) {
      const voltage = parseFloat(parts[2]);
      if (!isNaN(voltage)) {
        command.voltage = voltage;
      }
    }

    // Optional heading parameters
    if (parts.length >= 5) {
      const heading = parseFloat(parts[3]);
      const headingVoltage = parseFloat(parts[4]);
      if (!isNaN(heading) && !isNaN(headingVoltage)) {
        command.heading = heading;
        command.headingVoltage = headingVoltage;
      }
    }

    return command;
  }

  private parseTurnCommand(parts: string[]): ParsedCommand | null {
    if (parts.length < 2) {
      this.logError('Turn command requires heading parameter');
      return null;
    }

    const heading = parseFloat(parts[1]);
    if (isNaN(heading)) {
      this.logError('Invalid heading value');
      return null;
    }

    const command: ParsedCommand = {
      type: 'turn',
      heading: heading,
      voltage: 10 // default
    };

    // Optional voltage parameter
    if (parts.length >= 3) {
      const voltage = parseFloat(parts[2]);
      if (!isNaN(voltage)) {
        command.voltage = voltage;
      }
    }

    return command;
  }

  private formatRobotCommand(parsedCommand: ParsedCommand): string {
    switch (parsedCommand.type) {
      case 'drive':
        if (parsedCommand.heading !== undefined && parsedCommand.headingVoltage !== undefined) {
          return `DRIVE:${parsedCommand.distance}:${parsedCommand.voltage}:${parsedCommand.heading}:${parsedCommand.headingVoltage}\n`;
        } else {
          return `DRIVE:${parsedCommand.distance}:${parsedCommand.voltage}\n`;
        }
      case 'turn':
        return `TURN:${parsedCommand.heading}:${parsedCommand.voltage}\n`;
      default:
        return '';
    }
  }

  private formatChassisAPICall(parsedCommand: ParsedCommand): string {
    switch (parsedCommand.type) {
      case 'drive':
        if (parsedCommand.heading !== undefined && parsedCommand.headingVoltage !== undefined) {
          return `chassis.driveDistance(${parsedCommand.distance}, ${parsedCommand.voltage}, ${parsedCommand.heading}, ${parsedCommand.headingVoltage})`;
        } else {
          return `chassis.driveDistance(${parsedCommand.distance}, ${parsedCommand.voltage})`;
        }
      case 'turn':
        return `chassis.turnToHeading(${parsedCommand.heading}, ${parsedCommand.voltage})`;
      default:
        return '';
    }
  }

  private handleRobotResponse(data: string): void {
    if (data.startsWith('STATUS:')) {
      const statusParts = data.split(':');
      if (statusParts.length >= 3) {
        const heading = statusParts[1];
        const distance = statusParts[2];
        this.logResponse(`Robot Status - Heading: ${heading}°, Distance: ${distance} inches`);
      }
    } else {
      this.logResponse(data);
    }
  }

  private updateConnectionStatus(connected: boolean): void {
    this.statusIndicator.className = `status-dot ${connected ? 'connected' : 'disconnected'}`;
    this.statusText.textContent = connected ? 'Connected' : 'Disconnected';
  }

  private logCommand(command: string): void {
    const element = document.createElement('div');
    element.className = 'command';
    element.textContent = `> ${command}`;
    this.outputLog.appendChild(element);
    this.scrollToBottom();
  }

  private logResponse(response: string): void {
    const element = document.createElement('div');
    element.className = 'response';
    element.textContent = response;
    this.outputLog.appendChild(element);
    this.scrollToBottom();
  }

  private logError(error: string): void {
    const element = document.createElement('div');
    element.className = 'error';
    element.textContent = `Error: ${error}`;
    this.outputLog.appendChild(element);
    this.scrollToBottom();
  }

  private logStatus(status: string): void {
    const element = document.createElement('div');
    element.className = 'status';
    element.textContent = status;
    this.outputLog.appendChild(element);
    this.scrollToBottom();
  }

  private scrollToBottom(): void {
    this.outputLog.scrollTop = this.outputLog.scrollHeight;
  }

  private resetHistory(): void {
    this.outputLog.innerHTML = '';
    this.commandHistory = [];
    this.logStatus('Command history cleared');
  }

  private downloadAsCpp(): void {
    if (this.commandHistory.length === 0) {
      this.logError('No commands to download');
      return;
    }

    const cppContent = this.generateCppContent();
    const blob = new Blob([cppContent], { type: 'text/plain' });
    const url = URL.createObjectURL(blob);
    
    const a = document.createElement('a');
    a.href = url;
    a.download = 'autonTest.cpp';
    document.body.appendChild(a);
    a.click();
    document.body.removeChild(a);
    URL.revokeObjectURL(url);
    
    this.logStatus('Downloaded autonTest.cpp');
  }

  private generateCppContent(): string {
    let content = '// Auto-generated VEX auton test code\n';
    content += '// Generated from command line interface\n\n';
    content += 'void autonTest() {\n';
    
    for (const apiCall of this.commandHistory) {
      content += `    ${apiCall};\n`;
    }
    
    content += '}\n';
    
    return content;
  }
}

// Initialize the application when the page loads
document.addEventListener('DOMContentLoaded', () => {
  new CommandLineInterface();
}); 