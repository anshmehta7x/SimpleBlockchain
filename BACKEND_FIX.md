# C++ Backend Connection Fix

This document explains the fix for the C++ backend connection issue in the SimpleBlockchain Electron application.

## Problem

The C++ backend was starting correctly but exiting immediately when spawned from Electron, preventing the frontend from connecting. The process would show:

```
C++ process exited with code null
```

## Root Cause

The issue was caused by:

1. **Incorrect stdio configuration** - Using `['pipe', 'pipe', 'pipe']` caused the process to exit when stdin was closed
2. **Missing working directory** - The C++ process needed to run from its build directory
3. **Lack of signal handling** - The process could be terminated unexpectedly without graceful shutdown

## Solution

### 1. Fixed Electron Process Spawning

Updated `desktop-electron/src/main/index.js`:

```javascript
cppProcess = spawn(executablePath, ['--port', '3001'], {
  stdio: ['ignore', 'pipe', 'pipe'], // Don't pipe stdin
  detached: false,
  cwd: workingDir, // Set correct working directory
  windowsHide: true
})
```

### 2. Enhanced C++ Backend Signal Handling

Added to `SimpleBlockchain/src/SimpleBlockchain.cpp`:

```cpp
#include <signal.h>
#include <csignal>

// Signal handler for graceful shutdown
void signal_handler(int signal) {
    std::cout << "\nReceived signal " << signal << ". Shutting down gracefully..." << std::endl;
    shutdown_flag = 1;
}

int main(int argc, char *argv[]) {
    // Set up signal handlers
    std::signal(SIGTERM, signal_handler);
    std::signal(SIGINT, signal_handler);
    std::signal(SIGPIPE, SIG_IGN);
    
    // ... rest of main function
}
```

### 3. Improved Error Handling

Added better error reporting for HTTP server startup:

```cpp
if (!server.listen("127.0.0.1", port)) {
    std::cerr << "ERROR: Failed to start HTTP server on port " << port << std::endl;
    return 1;
}
```

## Testing

The fix can be verified by:

1. **Manual C++ backend test:**
   ```bash
   cd SimpleBlockchain/build
   ./simpleblockchain --port 3001
   ```

2. **Electron application:**
   ```bash
   cd desktop-electron
   npm run dev
   ```

3. **Connection test:**
   ```bash
   curl http://127.0.0.1:3001/api/blockchain
   ```

## Result

- ✅ C++ backend starts automatically with Electron
- ✅ HTTP server initializes correctly on port 3001
- ✅ Frontend can connect to backend successfully
- ✅ Process handles signals gracefully
- ✅ Enhanced error reporting and debugging

The educational blockchain application now works seamlessly without requiring manual backend startup.