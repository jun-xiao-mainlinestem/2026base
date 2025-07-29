export interface Point {
  x: number;
  y: number;
}

export interface RobotStatus {
  x: number;
  y: number;
  heading: number;
  distanceTraveled: number;
  isMoving: boolean;
}

export enum CommandType {
  FORWARD = 'FORWARD',
  BACKWARD = 'BACKWARD',
  TURN_LEFT = 'TURN_LEFT',
  TURN_RIGHT = 'TURN_RIGHT',
  TURN_CLOCKWISE = 'TURN_CLOCKWISE',
  TURN_COUNTERCLOCKWISE = 'TURN_COUNTERCLOCKWISE',
  STOP = 'STOP'
}

export interface Command {
  type: CommandType;
  value?: number; // For turning commands (degrees)
  timestamp: number;
}

export interface LogEvent {
  type: 'command' | 'movement' | 'status' | 'error';
  message: string;
  timestamp: number;
  data?: any;
}

export interface FieldDimensions {
  width: number;  // 144 inches
  height: number; // 144 inches
  robotLength: number; // 17 inches
  robotWidth: number;  // 15 inches
}

export interface MovementConfig {
  forwardSpeed: number;    // inches per frame
  backwardSpeed: number;   // inches per frame
  turnSpeed: number;       // degrees per frame (continuous)
  turn90Speed: number;     // degrees per frame (90-degree turns)
  animationFPS: number;    // target FPS
} 