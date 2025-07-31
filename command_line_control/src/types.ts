export interface WebSocketConfig {
  id: string;
  port: string;
  ip: string;
}

export interface RobotStatus {
  heading: number;
  distance: number;
}

export interface ParsedCommand {
  type: 'drive' | 'turn';
  distance?: number;
  voltage?: number;
  heading?: number;
  headingVoltage?: number;
}

export interface CommandResponse {
  success: boolean;
  message: string;
  data?: any;
} 