const { SerialPort } = require('serialport');
const readline = require('readline');

// Configuration
const PORT_NAME = '/dev/ttyUSB0'; // Change this for your system
const BAUD_RATE = 115200;

// Create readline interface for user input
const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

// Create serial port
const port = new SerialPort({
    path: PORT_NAME,
    baudRate: BAUD_RATE,
    autoOpen: false
});

// Available commands
const commands = {
    'MOVE_FORWARD': 'Drive forward 12 inches',
    'TURN_LEFT': 'Turn left 90 degrees',
    'TURN_RIGHT': 'Turn right 90 degrees',
    'STOP': 'Stop the robot',
    'STATUS': 'Get robot status',
    'help': 'Show this help message',
    'quit': 'Exit the program'
};

// Display help
function showHelp() {
    console.log('\nAvailable commands:');
    Object.entries(commands).forEach(([cmd, desc]) => {
        console.log(`  ${cmd} - ${desc}`);
    });
    console.log();
}

// Send command to VEX brain
function sendCommand(command) {
    if (port.isOpen) {
        port.write(command + '\n');
        console.log(`Sent: ${command}`);
    } else {
        console.log('Error: Port is not open');
    }
}

// Handle incoming data from VEX brain
port.on('data', (data) => {
    const message = data.toString().trim();
    if (message) {
        console.log(`VEX Response: ${message}`);
    }
});

// Handle port open
port.on('open', () => {
    console.log(`Connected to ${PORT_NAME} at ${BAUD_RATE} baud`);
    console.log('Press Button X on the VEX controller to start listening');
    showHelp();
    promptUser();
});

// Handle port errors
port.on('error', (err) => {
    console.error('Serial port error:', err.message);
    console.log('\nTroubleshooting:');
    console.log('1. Make sure the VEX brain is connected via USB');
    console.log('2. Check if the port name is correct');
    console.log('3. Try different port names:');
    console.log('   - Windows: COM1, COM2, COM3, etc.');
    console.log('   - Mac/Linux: /dev/ttyUSB0, /dev/ttyACM0, etc.');
    console.log('4. Make sure no other program is using the port');
    process.exit(1);
});

// Handle port close
port.on('close', () => {
    console.log('Serial connection closed');
    process.exit(0);
});

// Prompt user for input
function promptUser() {
    rl.question('Enter command: ', (input) => {
        const command = input.trim().toUpperCase();
        
        if (command === 'QUIT' || command === 'EXIT') {
            console.log('Exiting...');
            port.close();
            rl.close();
            return;
        }
        
        if (command === 'HELP') {
            showHelp();
        } else if (commands[command]) {
            sendCommand(command);
        } else if (command) {
            console.log(`Unknown command: ${command}`);
            console.log('Type "help" for available commands');
        }
        
        // Continue prompting
        promptUser();
    });
}

// Handle Ctrl+C
process.on('SIGINT', () => {
    console.log('\nExiting...');
    port.close();
    rl.close();
    process.exit(0);
});

// List available ports (useful for finding the correct port)
function listPorts() {
    SerialPort.list().then((ports) => {
        console.log('Available ports:');
        ports.forEach((port) => {
            console.log(`  ${port.path} - ${port.manufacturer || 'Unknown'}`);
        });
        console.log();
    }).catch((err) => {
        console.error('Error listing ports:', err);
    });
}

// Main execution
console.log('VEX V5 Serial Communication Test');
console.log('================================');

// List available ports first
listPorts();

// Try to open the port
console.log(`Attempting to connect to ${PORT_NAME}...`);
port.open((err) => {
    if (err) {
        console.error('Error opening port:', err.message);
        console.log('\nTry changing PORT_NAME in the script to one of the available ports above');
        process.exit(1);
    }
}); 