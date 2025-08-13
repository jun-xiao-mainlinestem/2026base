import { CommandType, DriveCommand, TurnCommand, SetHeadingCommand, ValidCommand } from './types';

export class CommandValidator {
  static validateDriveCommand(distance: number): DriveCommand | null {
    // Validate distance (any integer, can be negative)
    if (!Number.isInteger(distance)) {
      return null;
    }

    return {
      type: CommandType.DRIVE,
      parameters: { distance }
    };
  }

  static validateTurnCommand(heading: number): TurnCommand | null {
    // Validate heading (0 to 360)
    if (!Number.isInteger(heading) || heading < 0 || heading > 360) {
      return null;
    }

    return {
      type: CommandType.TURN,
      parameters: { heading }
    };
  }

  static validateSetHeadingCommand(heading: number): SetHeadingCommand | null {
    // Validate heading (0 to 360)
    if (!Number.isInteger(heading) || heading < 0 || heading > 360) {
      return null;
    }

    return {
      type: CommandType.SET_HEADING,
      parameters: { heading }
    };
  }
}

export class CommandFormatter {
  static formatCommand(command: ValidCommand): string {
    switch (command.type) {
      case CommandType.DRIVE:
        return `drive ${command.parameters.distance}\n`;
      
      case CommandType.TURN:
        return `turn ${command.parameters.heading}\n`;
      
      case CommandType.SET_HEADING:
        return `set_heading ${command.parameters.heading}\n`;
      
      default:
        throw new Error(`Unknown command type: ${(command as any).type}`);
    }
  }

  static getDefaultValues(commandType: CommandType): any {
    switch (commandType) {
      case CommandType.DRIVE:
        return { distance: 0 };
      case CommandType.TURN:
        return { heading: 0 };
      case CommandType.SET_HEADING:
        return { heading: 0 };
      default:
        return {};
    }
  }
}
