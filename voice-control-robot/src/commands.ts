import { VoiceCommand, RobotCommand, RobotAction } from './types';

export class CommandProcessor {
  private commandMap: Map<string, RobotAction> = new Map([
    ['forward', RobotAction.FORWARD],
    ['backward', RobotAction.BACKWARD],
    ['back', RobotAction.BACKWARD],
    ['left', RobotAction.LEFT],
    ['right', RobotAction.RIGHT],
    ['stop', RobotAction.STOP],
    ['intake', RobotAction.INTAKE],
    ['score', RobotAction.SCORE]
  ]);

  processVoiceCommand(voiceCommand: VoiceCommand): RobotCommand | null {
    const words = voiceCommand.command.split(' ');
    
    for (const word of words) {
      const action = this.commandMap.get(word);
      if (action) {
        return {
          action,
          parameters: this.extractParameters(voiceCommand.command, action)
        };
      }
    }
    
    return null;
  }

  private extractParameters(command: string, action: RobotAction): any {
    // Extract additional parameters like distance, speed, etc.
    const numbers = command.match(/\d+/g);
    if (numbers && numbers.length > 0) {
      return { value: parseInt(numbers[0]) };
    }
    return {};
  }

  getSupportedCommands(): string[] {
    return Array.from(this.commandMap.keys());
  }

  isValidCommand(command: string): boolean {
    return this.commandMap.has(command.toLowerCase());
  }
} 