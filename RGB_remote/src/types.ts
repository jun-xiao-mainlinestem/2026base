export interface ConnectionConfig {
  ipAddress: string;
  deviceId: string;
}

export interface Command {
  type: CommandType;
  parameters: CommandParameters;
}

export enum CommandType {
  DRIVE = 'drive',
  TURN = 'turn',
  SET_HEADING = 'set_heading'
}

export interface CommandParameters {
  distance?: number;
  voltage?: number; // Now always positive (1-12)
  heading?: number | null;
}

export interface DriveCommand extends Command {
  type: CommandType.DRIVE;
  parameters: {
    distance: number;
  };
}

export interface TurnCommand extends Command {
  type: CommandType.TURN;
  parameters: {
    heading: number;
  };
}

export interface SetHeadingCommand extends Command {
  type: CommandType.SET_HEADING;
  parameters: {
    heading: number;
  };
}

export type ValidCommand = DriveCommand | TurnCommand | SetHeadingCommand;

export interface ConnectionStatus {
  connected: boolean;
  connecting: boolean;
  error?: string;
}

export interface CommandHistory {
  timestamp: Date;
  command: string;
  success: boolean;
}
