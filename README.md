# Educational Blockchain Tool

An interactive blockchain application built with C++ backend and Electron frontend for learning blockchain technology.

## What is this?

This is an educational tool that demonstrates how blockchain technology works. It includes:

- **Complete Blockchain Visualization** - View the entire blockchain with real-time statistics
- **Block Explorer** - Examine individual blocks and transactions in detail  
- **Transaction Management** - Add, edit, and remove transactions from the pool
- **Blockchain Verification** - Check blockchain integrity with educational explanations
- **Tampering Demonstration** - See how blockchain detects unauthorized changes

## How to Run

1. **Install dependencies:**
   ```bash
   cd desktop-electron
   npm install
   ```

2. **Run the application:**
   ```bash
   npm run dev
   ```

   If you encounter issues, try the debug mode:
   ```bash
   npm run dev:debug
   ```

3. **Alternative - Browser version:**
   ```bash
   # Terminal 1: Start backend manually
   cd SimpleBlockchain/build && ./simpleblockchain --port 3001
   
   # Terminal 2: Serve frontend in browser
   cd desktop-electron && npm run serve:web
   # Then open http://localhost:5173
   ```

## Requirements

- Node.js and npm
- C++ compiler (g++ or clang++)
- CMake
- OpenSSL development libraries

The application will automatically build the C++ backend when you run it.