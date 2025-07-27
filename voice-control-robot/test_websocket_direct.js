const WebSocket = require('ws');

console.log('🔧 Direct WebSocket Test');
console.log('========================');
console.log('');

// Try different connection URLs based on USB port 1.0.0.75
const testUrls = [
  `ws://localhost:7071/vexrobotics.vexcode/device?id=51583`,
  `ws://localhost:7071/device?id=51583`,
  `ws://localhost:7071/vexrobotics.vexcode?id=51583`,
  `ws://localhost:7071/device/51583`,
  `ws://localhost:7071/vexrobotics.vexcode/device/51583`,
  `ws://1.0.0.75:7071/vexrobotics.vexcode/device?id=51583`,
  `ws://1.0.0.75:7071/device?id=51583`,
  `ws://1.0.0.75:7071/vexrobotics.vexcode?id=51583`,
  `ws://1.0.0.75:7071/device/51583`,
  `ws://1.0.0.75:7071/vexrobotics.vexcode/device/51583`
];

let currentTest = 0;

function testConnection(url) {
  console.log(`Testing URL: ${url}`);
  
  const ws = new WebSocket(url);
  
  ws.on('open', () => {
    console.log('✅ Connected!');
    console.log('📤 Sending test message...');
    ws.send('TEST');
    
    setTimeout(() => {
      console.log('⏱️  Test completed');
      ws.close();
      nextTest();
    }, 3000);
  });
  
  ws.on('message', (data) => {
    console.log(`📥 Received: ${data.toString()}`);
  });
  
  ws.on('error', (error) => {
    console.log(`❌ Error: ${error.message}`);
    nextTest();
  });
  
  ws.on('close', (code, reason) => {
    console.log(`🔌 Closed: ${code} - ${reason}`);
    nextTest();
  });
}

function nextTest() {
  currentTest++;
  if (currentTest < testUrls.length) {
    console.log('');
    console.log('🔄 Trying next URL...');
    console.log('');
    setTimeout(() => testConnection(testUrls[currentTest]), 1000);
  } else {
    console.log('');
    console.log('📊 All tests completed');
    console.log('');
    console.log('💡 If none worked, try:');
    console.log('1. Restart VS Code and VEX Extension');
    console.log('2. Check VEX Extension settings');
    console.log('3. Update VEX Extension to latest version');
    console.log('4. Try different Device ID');
  }
}

// Start testing
testConnection(testUrls[0]); 