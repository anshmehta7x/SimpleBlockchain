# Debugging the Electron Frontend Connection Issue

## Problem Summary
- ✅ Backend works perfectly when tested with curl
- ❌ Electron frontend shows "Failed to connect to blockchain server"

## Debugging Steps

### Step 1: Test Backend Manually
```bash
# Build the backend
cd SimpleBlockchain
mkdir -p build && cd build
cmake .. && make

# Start the backend manually
./simpleblockchain --port 3001
```

You should see:
```
Blockchain HTTP Server Starting...
Genesis Block Created 
...
HTTP server starting on port 3001...
```

### Step 2: Test API Connection
In another terminal:
```bash
curl http://127.0.0.1:3001/api/blockchain
```

You should get a JSON response with blockchain data.

### Step 3: Check Electron Build
```bash
cd ../desktop-electron
npm install
npm run build:cpp
```

This should build the C++ backend successfully.

### Step 4: Try Starting Electron (Debug Mode)

#### Option A: Try with Electron sandboxing disabled
Create a test script `debug-electron.js`:
```javascript
const { app, BrowserWindow } = require('electron');

app.commandLine.appendSwitch('--no-sandbox');
app.commandLine.appendSwitch('--disable-setuid-sandbox');

// Rest of your electron app code...
```

#### Option B: Try without Electron (Browser Test)
1. Build the React app first:
```bash
npx electron-vite build
```

2. Start the backend manually:
```bash
cd ../SimpleBlockchain/build
./simpleblockchain --port 3001
```

3. Serve the built React app on a local web server:
```bash
cd ../desktop-electron
npx serve out/renderer -l 5173
```

4. Open http://localhost:5173 in your browser

### Step 5: Check for Common Issues

#### Port Conflicts
```bash
# Check if port 3001 is already in use
lsof -i :3001
netstat -tulpn | grep :3001
```

#### Backend Executable Permissions
```bash
ls -la SimpleBlockchain/build/simpleblockchain
# Should show execute permissions (-rwxr-xr-x)
```

#### Path Issues in Electron
The main process looks for the executable at:
```
./SimpleBlockchain/build/simpleblockchain
```

Verify this path exists relative to the Electron app directory.

### Step 6: Enable Detailed Logging

Add this to your Electron main process (`src/main/index.js`):
```javascript
// Add detailed logging
app.on('ready', () => {
  console.log('App ready, executable path:', getExecutablePath());
  console.log('Executable exists:', require('fs').existsSync(getExecutablePath()));
});
```

## Likely Causes and Solutions

### 1. Timing Issue
The improved retry logic should handle this, but try increasing the delay:
```javascript
// In App.jsx, change to longer delay
setTimeout(() => tryConnection(1), 5000); // 5 seconds instead of 2
```

### 2. Electron Sandboxing (Most Likely)
The error message suggests sandboxing issues:
```
The SUID sandbox helper binary was found, but is not configured correctly
```

**Solution**: Run Electron with sandbox disabled:
```bash
cd desktop-electron
npm run dev -- --no-sandbox --disable-setuid-sandbox
```

Or add to package.json:
```json
{
  "scripts": {
    "dev:debug": "npm run build:cpp && electron-vite dev --no-sandbox --disable-setuid-sandbox"
  }
}
```

### 3. Path Resolution Issues
The executable path might be wrong. Try absolute paths:
```javascript
// In src/main/index.js
function getExecutablePath() {
  const path = require('path');
  if (is.dev) {
    return path.resolve(__dirname, '../../../SimpleBlockchain/build/simpleblockchain');
  }
  return path.join(process.resourcesPath, 'executables', 'simpleblockchain');
}
```

## Quick Fix to Try

1. **Run with sandbox disabled**:
```bash
cd desktop-electron
npm run build:cpp
electron --no-sandbox --disable-setuid-sandbox .
```

2. **Or use the browser version**:
```bash
# Terminal 1: Start backend
cd SimpleBlockchain/build && ./simpleblockchain --port 3001

# Terminal 2: Build and serve frontend  
cd desktop-electron
npx electron-vite build
npx serve out/renderer -l 5173
```

Then open http://localhost:5173 in your browser.

## Expected Result
With these fixes, you should see the Electron app successfully connect to the backend and display the blockchain interface instead of the "Server Error" message.