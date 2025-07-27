const { SerialPort } = require('serialport');
const readline = require('readline');

console.log('🔧 Direct Serial Communication Test');
console.log('==================================');
console.log('');

// Configuration
const BAUD_RATE = 115200;

// Create readline interface for user input
const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

// Available commands that match our robot commands
const commands = {
    'FORWARD': 'Drive forward',
    'BACKWARD': 'Drive backward', 
    'LEFT': 'Turn left',
    'RIGHT': 'Turn right',
    'STOP': 'Stop the robot',
    'INTAKE': 'Start intake roller',
    'SCORE': 'Start score roller',
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
function sendCommand(command, port) {
    if (port && port.isOpen) {
        port.write(command + '\n');
        console.log(`📤 Sent: ${command}`);
    } else {
        console.log('❌ Error: Port is not open');
    }
}

// Handle incoming data from VEX brain
function handleData(data) {
    const message = data.toString().trim();
    if (message) {
        console.log(`📥 VEX Response: ${message}`);
        
        // Parse robot status if it's in the expected format
        if (message.startsWith('STATUS:')) {
            const parts = message.split(':');
            if (parts.length >= 3) {
                const heading = parseFloat(parts[1]);
                const distance = parseFloat(parts[2]);
                console.log(`🤖 Robot Status:`);
                console.log(`   Heading: ${heading}°`);
                console.log(`   Distance: ${distance} inches`);
            }
        }
    }
}

// Handle port open
function handlePortOpen() {
    console.log(`✅ Connected to ${port.path} at ${BAUD_RATE} baud`);
    console.log('💡 Press Button X on the VEX controller to start listening');
    console.log('💡 Then type commands to test robot movement');
    showHelp();
    promptUser();
}

// Handle port errors
function handlePortError(err) {
    console.error('❌ Serial port error:', err.message);
    console.log('\n🔧 Troubleshooting:');
    console.log('1. Make sure the VEX brain is connected via USB');
    console.log('2. Check if the port name is correct');
    console.log('3. Try different port names:');
    console.log('   - Windows: COM1, COM2, COM3, etc.');
    console.log('   - Mac/Linux: /dev/ttyUSB0, /dev/ttyACM0, etc.');
    console.log('4. Make sure no other program is using the port');
    console.log('5. Try unplugging and reconnecting the USB cable');
    process.exit(1);
}

// Handle port close
function handlePortClose() {
    console.log('🔌 Serial connection closed');
    process.exit(0);
}

// Prompt user for input
function promptUser(port) {
    rl.question('Enter command: ', (input) => {
        const command = input.trim().toUpperCase();
        
        if (command === 'QUIT' || command === 'EXIT') {
            console.log('👋 Exiting...');
            if (port) port.close();
            rl.close();
            return;
        }
        
        if (command === 'HELP') {
            showHelp();
        } else if (commands[command]) {
            sendCommand(command, port);
        } else if (command) {
            console.log(`❌ Unknown command: ${command}`);
            console.log('💡 Type "help" for available commands');
        }
        
        // Continue prompting
        promptUser(port);
    });
}

// Handle Ctrl+C
process.on('SIGINT', () => {
    console.log('\n👋 Exiting...');
    if (port) port.close();
    rl.close();
    process.exit(0);
});

// List available ports
async function listPorts() {
    try {
        const ports = await SerialPort.list();
        console.log('🔍 Available ports:');
        ports.forEach((port) => {
            console.log(`  ${port.path} - ${port.manufacturer || 'Unknown'} (${port.pnpId || 'No ID'})`);
        });
        console.log('');
        
        // Try to find VEX-related ports
        const vexPorts = ports.filter(p => 
            p.manufacturer && p.manufacturer.toLowerCase().includes('vex') ||
            p.path.includes('tty.usb') ||
            p.path.includes('COM')
        );
        
        if (vexPorts.length > 0) {
            console.log('🎯 Potential VEX ports:');
            vexPorts.forEach(p => console.log(`  ${p.path} - ${p.manufacturer || 'Unknown'}`));
            console.log('');
        }
        
        return ports;
    } catch (err) {
        console.error('❌ Error listing ports:', err);
        return [];
    }
}

// Main execution
async function main() {
    console.log('🤖 VEX V5 Direct Serial Communication Test');
    console.log('==========================================');
    console.log('');
    
    // List available ports first
    const ports = await listPorts();
    
    if (ports.length === 0) {
        console.log('❌ No serial ports found');
        console.log('💡 Make sure the VEX brain is connected via USB');
        process.exit(1);
    }
    
    // Try to find the VEX port automatically
    let portPath = null;
    
    // Look for common VEX port patterns
    const potentialPorts = ports.filter(p => 
        p.path.includes('tty.usb') ||
        p.path.includes('ttyACM') ||
        p.path.includes('COM')
    );
    
    if (potentialPorts.length > 0) {
        portPath = potentialPorts[0].path;
        console.log(`🎯 Auto-selected port: ${portPath}`);
    } else {
        console.log('❌ No suitable port found automatically');
        console.log('💡 Please specify a port manually');
        process.exit(1);
    }
    
    // Create serial port
    const port = new SerialPort({
        path: portPath,
        baudRate: BAUD_RATE,
        autoOpen: false
    });
    
    // Set up event handlers with proper scope
    port.on('data', (data) => handleData(data));
    port.on('open', () => {
        console.log(`✅ Connected to ${port.path} at ${BAUD_RATE} baud`);
        console.log('💡 Press Button X on the VEX controller to start listening');
        console.log('💡 Then type commands to test robot movement');
        showHelp();
        promptUser(port);
    });
    port.on('error', (err) => {
        console.error('❌ Serial port error:', err.message);
        console.log('\n🔧 Troubleshooting:');
        console.log('1. Make sure the VEX brain is connected via USB');
        console.log('2. Check if the port name is correct');
        console.log('3. Try different port names:');
        console.log('   - Windows: COM1, COM2, COM3, etc.');
        console.log('   - Mac/Linux: /dev/ttyUSB0, /dev/ttyACM0, etc.');
        console.log('4. Make sure no other program is using the port');
        console.log('5. Try unplugging and reconnecting the USB cable');
        process.exit(1);
    });
    port.on('close', () => {
        console.log('🔌 Serial connection closed');
        process.exit(0);
    });
    
    // Try to open the port
    console.log(`🔌 Attempting to connect to ${portPath}...`);
    port.open((err) => {
        if (err) {
            console.error('❌ Error opening port:', err.message);
            console.log('\n💡 Try changing the port path in the script');
            process.exit(1);
        }
    });
}

// Start the test
main().catch(console.error); 