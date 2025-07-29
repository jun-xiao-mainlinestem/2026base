import { Field } from './field';
import { Robot } from './robot';
import { CommandSystem } from './commands';
import { StatusLogger } from './status-logger';
import { CommandType, Command } from './types';
import './styles.css';

export class RobotSimulation {
  private canvas: HTMLCanvasElement;
  private field: Field;
  private robot: Robot;
  private commandSystem: CommandSystem;
  private statusLogger: StatusLogger;
  private animationId: number | null = null;
  private isDragging: boolean = false;
  private dragOffset: { x: number; y: number } = { x: 0, y: 0 };

  constructor() {
    this.canvas = document.getElementById('field-canvas') as HTMLCanvasElement;
    this.field = new Field(this.canvas);
    this.robot = new Robot(this.field);
    this.statusLogger = new StatusLogger(this.robot);
    this.commandSystem = new CommandSystem(this.robot, this.statusLogger);

    this.initialize();
  }

  private async initialize(): Promise<void> {
    try {
      // Load field image - try multiple paths for different environments
      try {
        await this.field.loadImage('/assets/topdown-view-field.png');
      } catch (error) {
        console.log('Failed to load image from /assets/, trying relative path...');
        await this.field.loadImage('./assets/topdown-view-field.png');
      }
      
      this.setupEventListeners();
      this.startAnimation();
      this.updateStatus();
      
      this.statusLogger.logEvent({
        type: 'status',
        message: 'Robot simulation initialized successfully',
        timestamp: Date.now()
      });
    } catch (error) {
      console.error('Failed to initialize simulation:', error);
      this.statusLogger.logError(`Initialization failed: ${error}`);
    }
  }

  private setupEventListeners(): void {
    // Command buttons
    document.getElementById('forward-btn')?.addEventListener('click', () => this.executeCommand(CommandType.FORWARD));
    document.getElementById('backward-btn')?.addEventListener('click', () => this.executeCommand(CommandType.BACKWARD));
    document.getElementById('turn-left-btn')?.addEventListener('click', () => this.executeCommand(CommandType.TURN_LEFT));
    document.getElementById('turn-right-btn')?.addEventListener('click', () => this.executeCommand(CommandType.TURN_RIGHT));
    document.getElementById('turn-cw-btn')?.addEventListener('click', () => this.executeCommand(CommandType.TURN_CLOCKWISE));
    document.getElementById('turn-ccw-btn')?.addEventListener('click', () => this.executeCommand(CommandType.TURN_COUNTERCLOCKWISE));
    document.getElementById('stop-btn')?.addEventListener('click', () => this.executeCommand(CommandType.STOP));

    // Clear log button
    document.getElementById('clear-log-btn')?.addEventListener('click', () => this.clearLog());

    // Mouse events for dragging
    this.canvas.addEventListener('mousedown', (e) => this.handleMouseDown(e));
    this.canvas.addEventListener('mousemove', (e) => this.handleMouseMove(e));
    this.canvas.addEventListener('mouseup', () => this.handleMouseUp());

    // Touch events for mobile
    this.canvas.addEventListener('touchstart', (e) => this.handleTouchStart(e));
    this.canvas.addEventListener('touchmove', (e) => this.handleTouchMove(e));
    this.canvas.addEventListener('touchend', () => this.handleTouchEnd());

    // Wheel event for rotation
    this.canvas.addEventListener('wheel', (e) => this.handleWheel(e));

    // Keyboard events
    document.addEventListener('keydown', (e) => this.handleKeyDown(e));
  }

  private executeCommand(commandType: CommandType): void {
    const command: Command = {
      type: commandType,
      timestamp: Date.now()
    };

    this.commandSystem.addCommand(command);
    
    // Log the command
    const commandNames: Record<CommandType, string> = {
      [CommandType.FORWARD]: 'Drive Forward',
      [CommandType.BACKWARD]: 'Drive Backward',
      [CommandType.TURN_LEFT]: 'Turn Left (90°)',
      [CommandType.TURN_RIGHT]: 'Turn Right (90°)',
      [CommandType.TURN_CLOCKWISE]: 'Turn Clockwise',
      [CommandType.TURN_COUNTERCLOCKWISE]: 'Turn Counter-clockwise',
      [CommandType.STOP]: 'Stop'
    };

    this.statusLogger.logCommand(commandNames[commandType]);
  }

  private handleMouseDown(e: MouseEvent): void {
    const rect = this.canvas.getBoundingClientRect();
    const x = e.clientX - rect.left;
    const y = e.clientY - rect.top;
    
    const robotPos = this.robot.getPosition();
    const robotScreenPos = this.field.fieldToScreen(robotPos.x, robotPos.y);
    
    // Calculate robot dimensions using the same method as the drawing
    const fieldDimensions = this.field.getDimensions();
    const fieldScreenPos1 = this.field.fieldToScreen(0, 0);
    const fieldScreenPos2 = this.field.fieldToScreen(fieldDimensions.width, fieldDimensions.height);
    const fieldScreenWidth = fieldScreenPos2.x - fieldScreenPos1.x;
    const screenLength = (fieldDimensions.robotLength / fieldDimensions.width) * fieldScreenWidth;
    const screenWidth = (fieldDimensions.robotWidth / fieldDimensions.width) * fieldScreenWidth;

    // Check if click is within robot bounds (rectangular)
    const dx = x - robotScreenPos.x;
    const dy = y - robotScreenPos.y;
    
    // Use rectangular bounds check
    if (Math.abs(dx) <= screenLength / 2 && Math.abs(dy) <= screenWidth / 2) {
      this.isDragging = true;
      this.dragOffset = { x: dx, y: dy };
      this.canvas.style.cursor = 'grabbing';
    }
  }

  private handleMouseMove(e: MouseEvent): void {
    if (!this.isDragging) return;

    const rect = this.canvas.getBoundingClientRect();
    const x = e.clientX - rect.left - this.dragOffset.x;
    const y = e.clientY - rect.top - this.dragOffset.y;
    
    const fieldPos = this.field.screenToField(x, y);
    this.robot.setPosition(fieldPos.x, fieldPos.y);
  }

  private handleMouseUp(): void {
    if (this.isDragging) {
      this.isDragging = false;
      this.canvas.style.cursor = 'crosshair';
      
      // Update the last stop position to the new dragged position
      // This ensures distance calculation starts from the new position
      this.statusLogger.resetDistance();
      
      this.statusLogger.logEvent({
        type: 'status',
        message: `Robot moved to position (${this.robot.getPosition().x.toFixed(1)}, ${this.robot.getPosition().y.toFixed(1)})`,
        timestamp: Date.now()
      });
    }
  }

  private handleTouchStart(e: TouchEvent): void {
    e.preventDefault();
    const touch = e.touches[0];
    const rect = this.canvas.getBoundingClientRect();
    const x = touch.clientX - rect.left;
    const y = touch.clientY - rect.top;
    
    const robotPos = this.robot.getPosition();
    const robotScreenPos = this.field.fieldToScreen(robotPos.x, robotPos.y);
    
    // Calculate robot dimensions using the same method as the drawing
    const fieldDimensions = this.field.getDimensions();
    const fieldScreenPos1 = this.field.fieldToScreen(0, 0);
    const fieldScreenPos2 = this.field.fieldToScreen(fieldDimensions.width, fieldDimensions.height);
    const fieldScreenWidth = fieldScreenPos2.x - fieldScreenPos1.x;
    const screenLength = (fieldDimensions.robotLength / fieldDimensions.width) * fieldScreenWidth;
    const screenWidth = (fieldDimensions.robotWidth / fieldDimensions.width) * fieldScreenWidth;

    const dx = x - robotScreenPos.x;
    const dy = y - robotScreenPos.y;
    
    // Use rectangular bounds check
    if (Math.abs(dx) <= screenLength / 2 && Math.abs(dy) <= screenWidth / 2) {
      this.isDragging = true;
      this.dragOffset = { x: dx, y: dy };
    }
  }

  private handleTouchMove(e: TouchEvent): void {
    if (!this.isDragging) return;
    e.preventDefault();

    const touch = e.touches[0];
    const rect = this.canvas.getBoundingClientRect();
    const x = touch.clientX - rect.left - this.dragOffset.x;
    const y = touch.clientY - rect.top - this.dragOffset.y;
    
    const fieldPos = this.field.screenToField(x, y);
    this.robot.setPosition(fieldPos.x, fieldPos.y);
  }

  private handleTouchEnd(): void {
    if (this.isDragging) {
      this.isDragging = false;
      
      // Update the last stop position to the new dragged position
      // This ensures distance calculation starts from the new position
      this.statusLogger.resetDistance();
      
      this.statusLogger.logEvent({
        type: 'status',
        message: `Robot moved to position (${this.robot.getPosition().x.toFixed(1)}, ${this.robot.getPosition().y.toFixed(1)})`,
        timestamp: Date.now()
      });
    }
  }

  private handleWheel(e: WheelEvent): void {
    // Only handle wheel events when mouse is over the robot
    const rect = this.canvas.getBoundingClientRect();
    const mouseX = e.clientX - rect.left;
    const mouseY = e.clientY - rect.top;
    
    // Convert mouse position to field coordinates
    const fieldPos = this.field.screenToField(mouseX, mouseY);
    
    // Get robot position and dimensions
    const robotPos = this.robot.getPosition();
    const fieldDimensions = this.field.getDimensions();
    const halfLength = fieldDimensions.robotLength / 2;
    const halfWidth = fieldDimensions.robotWidth / 2;
    
    // Check if mouse is within robot bounds (rectangular)
    if (fieldPos.x >= robotPos.x - halfLength && fieldPos.x <= robotPos.x + halfLength &&
        fieldPos.y >= robotPos.y - halfWidth && fieldPos.y <= robotPos.y + halfWidth) {
      e.preventDefault();
      const rotationAmount = e.deltaY > 0 ? 15 : -15; // 15 degrees per scroll
      this.robot.setHeading(this.robot.getHeading() + rotationAmount);
      
      this.statusLogger.logEvent({
        type: 'status',
        message: `Robot rotated to ${this.robot.getHeading().toFixed(1)}°`,
        timestamp: Date.now()
      });
    }
  }

  private handleKeyDown(e: KeyboardEvent): void {
    switch (e.key) {
      case 'ArrowUp':
      case 'w':
        this.executeCommand(CommandType.FORWARD);
        break;
      case 'ArrowDown':
      case 's':
        this.executeCommand(CommandType.BACKWARD);
        break;
      case 'ArrowLeft':
      case 'a':
        this.executeCommand(CommandType.TURN_LEFT);
        break;
      case 'ArrowRight':
      case 'd':
        this.executeCommand(CommandType.TURN_RIGHT);
        break;
      case 'q':
        this.executeCommand(CommandType.TURN_COUNTERCLOCKWISE);
        break;
      case 'e':
        this.executeCommand(CommandType.TURN_CLOCKWISE);
        break;
      case ' ':
        e.preventDefault();
        this.executeCommand(CommandType.STOP);
        break;
    }
  }

  private startAnimation(): void {
    const animate = () => {
      this.render();
      this.updateStatus();
      this.commandSystem.updateContinuousMovement();
      this.animationId = requestAnimationFrame(animate);
    };
    animate();
  }

  private render(): void {
    this.field.draw();
    this.robot.draw(this.field.getContext());
  }

  private updateStatus(): void {
    this.statusLogger.updateStatus();
    const status = this.statusLogger.getCurrentStatus();

    // Update status display
    const headingDisplay = document.getElementById('heading-display');
    const distanceDisplay = document.getElementById('distance-display');
    const movementStatus = document.getElementById('movement-status');

    if (headingDisplay) {
      headingDisplay.textContent = `${status.heading.toFixed(1)}°`;
    }
    if (distanceDisplay) {
      distanceDisplay.textContent = `${status.distanceTraveled.toFixed(1)} inches`;
    }
    if (movementStatus) {
      movementStatus.textContent = status.isMoving ? 'Moving' : 'Stopped';
      movementStatus.className = status.isMoving ? 'moving' : 'stopped';
    }

    // Update log display
    this.updateLogDisplay();
  }

  private updateLogDisplay(): void {
    const logDisplay = document.getElementById('log-display');
    if (!logDisplay) return;

    const logEvents = this.statusLogger.getFormattedLog();
    logDisplay.innerHTML = logEvents.map(event => `<div>${event}</div>`).join('');
    logDisplay.scrollTop = logDisplay.scrollHeight;
  }

  private clearLog(): void {
    this.statusLogger.clearLog();
    this.updateLogDisplay();
  }
}

// Initialize the simulation when the DOM is loaded
document.addEventListener('DOMContentLoaded', () => {
  new RobotSimulation();
}); 