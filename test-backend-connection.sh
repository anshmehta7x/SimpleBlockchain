#!/bin/bash

# Test script to verify C++ backend connection fix
# This script tests the same spawn configuration used by Electron

echo "Testing C++ Backend Connection Fix"
echo "=================================="

# Build the C++ backend first
echo "Building C++ backend..."
cd "$(dirname "$0")/SimpleBlockchain"
mkdir -p build
cd build
cmake .. && make

if [ $? -ne 0 ]; then
    echo "❌ C++ build failed"
    exit 1
fi

echo "✅ C++ build successful"

# Test the backend with Node.js spawn (mimicking Electron)
echo "Testing spawn configuration..."
cd "$(dirname "$0")"

node -e "
const { spawn } = require('child_process');
const { dirname } = require('path');

// Use exact path as found in the system
const executablePath = process.cwd() + '/SimpleBlockchain/build/simpleblockchain';
const workingDir = dirname(executablePath);

console.log('Executable path:', executablePath);
console.log('Working directory:', workingDir);

const cppProcess = spawn(executablePath, ['--port', '3001'], {
  stdio: ['ignore', 'pipe', 'pipe'],
  detached: false,
  cwd: workingDir
});

let serverStarted = false;
let connectionTested = false;

cppProcess.stdout.on('data', (data) => {
  const output = data.toString();
  console.log('Backend:', output.trim());
  
  if (output.includes('HTTP server starting on port 3001')) {
    serverStarted = true;
    setTimeout(testConnection, 2000);
  }
});

cppProcess.stderr.on('data', (data) => {
  console.error('Backend error:', data.toString().trim());
});

cppProcess.on('close', (code, signal) => {
  console.log(\`Process exited with code \${code}, signal \${signal}\`);
  if (!connectionTested) {
    console.log('❌ Process exited before connection could be tested');
    process.exit(1);
  }
});

cppProcess.on('error', (error) => {
  console.error('❌ Failed to start process:', error.message);
  process.exit(1);
});

function testConnection() {
  if (connectionTested) return;
  connectionTested = true;
  
  console.log('Testing HTTP connection...');
  const http = require('http');
  
  const req = http.get('http://127.0.0.1:3001/api/blockchain', (res) => {
    console.log('✅ Connection successful! Status:', res.statusCode);
    
    let data = '';
    res.on('data', chunk => data += chunk);
    res.on('end', () => {
      try {
        const response = JSON.parse(data);
        console.log('✅ Backend response valid JSON with', response.blocks?.length || 0, 'blocks');
        console.log('✅ All tests passed! Backend is working correctly.');
        
        // Kill the process and exit
        cppProcess.kill();
        setTimeout(() => process.exit(0), 100);
      } catch (e) {
        console.error('❌ Invalid JSON response:', e.message);
        process.exit(1);
      }
    });
  });
  
  req.on('error', (err) => {
    console.error('❌ Connection failed:', err.message);
    process.exit(1);
  });
  
  req.setTimeout(5000, () => {
    console.error('❌ Connection timed out');
    req.destroy();
    process.exit(1);
  });
}

// Timeout if server doesn't start
setTimeout(() => {
  if (!serverStarted) {
    console.error('❌ Server did not start within 10 seconds');
    cppProcess.kill();
    process.exit(1);
  }
}, 10000);
"

echo ""
echo "Test completed. If you see '✅ All tests passed!' above, the fix is working correctly."
echo "You can now run 'cd desktop-electron && npm run dev' to start the full application."