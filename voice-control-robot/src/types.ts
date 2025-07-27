export interface VoiceCommand {
  command: string;
  confidence: number;
  timestamp: Date;
}

export interface RobotCommand {
  action: RobotAction;
  parameters?: any;
}

export enum RobotAction {
  FORWARD = 'FORWARD',
  BACKWARD = 'BACKWARD',
  LEFT = 'LEFT',
  RIGHT = 'RIGHT',
  STOP = 'STOP',
  INTAKE = 'INTAKE',
  SCORE = 'SCORE'
}

export interface WebSocketConfig {
  id: string;
  port: string;
  ip: string;
}

export interface RobotStatus {
  heading: number;
  distance: number;
} 