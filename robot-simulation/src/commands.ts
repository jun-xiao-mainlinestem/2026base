import { Command, CommandType, MovementConfig } from './types';
import { Robot } from './robot';

export class CommandSystem {
  private robot: Robot;
  private commandQueue: Command[] = [];
  private isExecuting: boolean = false;
  private currentCommand: Command | null = null;
  private movementConfig: MovementConfig;
  private statusLogger: any; // Reference to status logger
  private continuousCommands: Set<CommandType> = new Set([
    CommandType.FORWARD,
    CommandType.BACKWARD,
    CommandType.TURN_CLOCKWISE,
    CommandType.TURN_COUNTERCLOCKWISE
  ]);

  constructor(robot: Robot, statusLogger?: any) {
    this.robot = robot;
    this.movementConfig = robot.getMovementConfig();
    this.statusLogger = statusLogger;
  }

  public addCommand(command: Command): void {
    // Handle stop command immediately
    if (command.type === CommandType.STOP) {
      this.robot.setMoving(false);
      this.currentCommand = null;
      this.isExecuting = false;
      this.commandQueue = []; // Clear all queued commands
      // Don't reset distance - let it be displayed
      return;
    }
    
    // For movement commands, reset distance when starting to move
    if (this.continuousCommands.has(command.type) || 
        command.type === CommandType.TURN_LEFT || 
        command.type === CommandType.TURN_RIGHT) {
      // Reset distance when starting a new movement
      if (this.statusLogger) {
        this.statusLogger.resetDistance();
      }
    }
    
    // For continuous commands, replace current command
    if (this.continuousCommands.has(command.type)) {
      this.currentCommand = command;
      this.isExecuting = true;
      this.robot.setMoving(true);
    } else {
      // For single commands, add to queue
      this.commandQueue.push(command);
      
      // If not currently executing, start execution
      if (!this.isExecuting) {
        this.executeNextCommand();
      }
    }
  }

  public executeCommand(command: Command): void {
    this.currentCommand = command;
    this.robot.setMoving(true);

    switch (command.type) {
      case CommandType.FORWARD:
        this.executeForward();
        break;
      case CommandType.BACKWARD:
        this.executeBackward();
        break;
      case CommandType.TURN_LEFT:
        this.executeTurnLeft();
        break;
      case CommandType.TURN_RIGHT:
        this.executeTurnRight();
        break;
      case CommandType.TURN_CLOCKWISE:
        this.executeTurnClockwise();
        break;
      case CommandType.TURN_COUNTERCLOCKWISE:
        this.executeTurnCounterclockwise();
        break;
      case CommandType.STOP:
        this.executeStop();
        break;
    }
  }

  private executeForward(): void {
    const distance = this.movementConfig.forwardSpeed;
    this.robot.move(distance);
    // Don't complete for continuous movement
  }

  private executeBackward(): void {
    const distance = this.movementConfig.backwardSpeed;
    this.robot.move(-distance);
    // Don't complete for continuous movement
  }

  private executeTurnLeft(): void {
    this.robot.turn(-90);
    this.completeCommand(); // Complete for 90-degree turns
  }

  private executeTurnRight(): void {
    this.robot.turn(90);
    this.completeCommand(); // Complete for 90-degree turns
  }

  private executeTurnClockwise(): void {
    const angle = this.movementConfig.turnSpeed;
    this.robot.turn(angle);
    // Don't complete for continuous movement
  }

  private executeTurnCounterclockwise(): void {
    const angle = this.movementConfig.turnSpeed;
    this.robot.turn(-angle);
    // Don't complete for continuous movement
  }

  private executeStop(): void {
    this.robot.setMoving(false);
    this.currentCommand = null;
    this.isExecuting = false;
    // Don't call completeCommand() as it would execute the next command in queue
  }

  private completeCommand(): void {
    this.currentCommand = null;
    this.isExecuting = false;
    
    // Execute next command if available
    if (this.commandQueue.length > 0) {
      this.executeNextCommand();
    }
  }

  private executeNextCommand(): void {
    if (this.commandQueue.length === 0) {
      this.isExecuting = false;
      return;
    }

    this.isExecuting = true;
    const command = this.commandQueue.shift()!;
    this.executeCommand(command);
  }

  public clearQueue(): void {
    this.commandQueue = [];
    this.isExecuting = false;
    this.currentCommand = null;
    this.robot.setMoving(false);
  }

  public getIsExecuting(): boolean {
    return this.isExecuting;
  }

  public getQueueLength(): number {
    return this.commandQueue.length;
  }

  public getCurrentCommand(): Command | null {
    return this.currentCommand;
  }

  public updateMovementConfig(config: Partial<MovementConfig>): void {
    this.movementConfig = { ...this.movementConfig, ...config };
    this.robot.updateMovementConfig(config);
  }

  public updateContinuousMovement(): void {
    if (this.currentCommand && this.continuousCommands.has(this.currentCommand.type)) {
      this.executeCommand(this.currentCommand);
    }
  }
} 