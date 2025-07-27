const WebSocket = require('ws');
const readline = require('readline');

console.log('🔧 Simple WebSocket Test (Single-Character Commands)');
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

  // Sequence of single-character commands to test
  const testSequence = [
    { label: 'FORWARD', char: 'a', delay: 2000 },
    { label: 'STOP', char: 'p', delay: 1000 },
    { label: 'RIGHT', char: 'd', delay: 2000 },
    { label: 'STOP', char: 'p', delay: 1000 },
    { label: 'LEFT', char: 'l', delay: 2000 },
    { label: 'STOP', char: 'p', delay: 1000 },
    { label: 'BACKWARD', char: 'b', delay: 2000 },
    { label: 'STOP', char: 'p', delay: 1000 },
    { label: 'INTAKE', char: 'i', delay: 2000 },
    { label: 'SCORE', char: 's', delay: 2000 },
    { label: 'STOP', char: 'p', delay: 1000 }
  ];

  let currentIndex = 0;

  function sendNextCommand() {
    if (currentIndex < testSequence.length) {
      const cmd = testSequence[currentIndex];
      console.log(`📤 Sending: ${cmd.label} (char: '${cmd.char}')`);
      ws.send(cmd.char);
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