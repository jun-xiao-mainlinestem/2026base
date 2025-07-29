const WebSocket = require('ws');
const readline = require('readline');

console.log('🔧 Simple WebSocket Test (Text Commands)');
console.log('==================================');
console.log('');

// Accept device id from command line or prompt
let deviceId = process.argv[2];

function promptForId(callback) {
  const rl = readline.createInterface({ input: process.stdin, output: process.stdout });
  rl.question('Enter VEX Device ID (default: 77606): ', (answer) => {
    rl.close();
    callback(answer && answer.trim() ? answer.trim() : '77606');
  });
}

function startTest(id) {
  // Configuration
  const config = {
    id: id,
    port: "7071",
    ip: "localhost"
  };

  const url = `ws://${config.ip}:${config.port}/vexrobotics.vexcode/device?id=${config.id}`;

  console.log(`Connecting to: ${url}`);
  console.log('');

  let ws = null;
  let testResults = {
    connected: false,
    messageSent: false,
    messageReceived: false
  };

  // Sequence of text commands to test
  const testSequence = [
    { label: 'FORWARD', command: 'FORWARD', delay: 2000 },
    { label: 'STOP', command: 'STOP', delay: 1000 },
    { label: 'RIGHT', command: 'RIGHT', delay: 2000 },
    { label: 'STOP', command: 'STOP', delay: 1000 },
    { label: 'LEFT', command: 'LEFT', delay: 2000 },
    { label: 'STOP', command: 'STOP', delay: 1000 },
    { label: 'BACKWARD', command: 'BACKWARD', delay: 2000 },
    { label: 'STOP', command: 'STOP', delay: 1000 },
    { label: 'ROLL', command: 'ROLL', delay: 2000 },
    { label: 'SHOOT', command: 'SHOOT', delay: 2000 },
    { label: 'STOP', command: 'STOP', delay: 1000 }
  ];

  let currentIndex = 0;

  function sendNextCommand() {
    if (currentIndex < testSequence.length) {
      const cmd = testSequence[currentIndex];
      console.log(`📤 Sending: ${cmd.label} (command: '${cmd.command}')`);
      ws.send(cmd.command);
      testResults.messageSent = true;
      currentIndex++;
      setTimeout(sendNextCommand, cmd.delay);
    } else {
      console.log('');
      console.log('✅ Test sequence completed!');
      ws.close();
    }
  }

  function runTest() {
    ws = new WebSocket(url);
    
    ws.on('open', () => {
      console.log('✅ WebSocket connected!');
      testResults.connected = true;
      setTimeout(sendNextCommand, 1000);
    });
    
    ws.on('message', (data) => {
      console.log(`📥 Received: ${data.toString()}`);
      testResults.messageReceived = true;
    });
    
    ws.on('error', (error) => {
      console.error('❌ WebSocket error:', error.message);
      console.log('💡 Make sure VEX Extension is running and WebSocket Server is enabled');
    });
    
    ws.on('close', (code, reason) => {
      console.log(`🔌 WebSocket closed. Code: ${code}, Reason: ${reason}`);
      console.log('');
      console.log('📊 Test Results:');
      console.log(`   WebSocket Connected: ${testResults.connected ? '✅' : '❌'}`);
      console.log(`   Message Sent: ${testResults.messageSent ? '✅' : '❌'}`);
      console.log(`   Message Received: ${testResults.messageReceived ? '✅' : '❌'}`);
      console.log('');
      console.log('💡 Check the VEX controller screen and robot movement for each command.');
    });
  }

  console.log('⏳ Waiting for connection...');
  runTest();
}

if (deviceId) {
  startTest(deviceId);
} else {
  promptForId(startTest);
} 