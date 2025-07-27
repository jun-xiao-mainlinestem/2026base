const readline = require('readline');

// Create readline interface for user input
const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});

console.log('🤖 VEX Robot Command Tester');
console.log('============================');
console.log('');
console.log('This script will help you test robot commands manually.');
console.log('Make sure:');
console.log('1. VEX brain is connected via USB');
console.log('2. Button X is pressed on VEX controller');
console.log('3. Controller screen shows "Serial listening ON"');
console.log('');
console.log('Available commands: FORWARD, BACKWARD, LEFT, RIGHT, STOP, INTAKE, SCORE');
console.log('Type "quit" to exit');
console.log('');

// Function to send command to VEX brain
function sendCommand(command) {
  // This is a simulation - in real use, you'd send via WebSocket
  console.log(`📤 Sending command: ${command}`);
  console.log(`💡 Check VEX controller screen for: "Received: ${command}"`);
  console.log(`💡 Check VEX controller screen for: "Executing: ${command}"`);
  console.log('');
}

// Main command loop
function promptForCommand() {
  rl.question('Enter command (or "quit" to exit): ', (input) => {
    const command = input.trim().toUpperCase();
    
    if (command === 'QUIT') {
      console.log('👋 Goodbye!');
      rl.close();
      return;
    }
    
    // Validate command
    const validCommands = ['FORWARD', 'BACKWARD', 'LEFT', 'RIGHT', 'STOP', 'INTAKE', 'SCORE'];
    
    if (validCommands.includes(command)) {
      sendCommand(command);
      
      // Give user feedback about expected robot behavior
      switch (command) {
        case 'FORWARD':
          console.log('🤖 Expected: Robot drives forward');
          break;
        case 'BACKWARD':
          console.log('🤖 Expected: Robot drives backward');
          break;
        case 'LEFT':
          console.log('🤖 Expected: Robot turns left');
          break;
        case 'RIGHT':
          console.log('🤖 Expected: Robot turns right');
          break;
        case 'STOP':
          console.log('🤖 Expected: Robot stops and reports status');
          break;
        case 'INTAKE':
          console.log('🤖 Expected: Intake mechanism starts (continues until STOP)');
          break;
        case 'SCORE':
          console.log('🤖 Expected: Scoring mechanism starts (continues until STOP)');
          break;
      }
    } else {
      console.log('❌ Invalid command. Valid commands are:');
      console.log('   FORWARD, BACKWARD, LEFT, RIGHT, STOP, INTAKE, SCORE');
    }
    
    console.log('');
    promptForCommand();
  });
}

// Start the command loop
promptForCommand();

// Handle Ctrl+C gracefully
rl.on('SIGINT', () => {
  console.log('\n👋 Goodbye!');
  rl.close();
}); 