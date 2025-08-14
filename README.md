# Educational Blockchain Tool

A comprehensive educational blockchain application built with C++ backend and Electron frontend to help learn and explore blockchain technology.

## Features

### 🔗 Complete Blockchain Implementation
- **Genesis Block Creation**: Automatically creates the first block in the chain
- **Mining with Proof of Work**: Configurable difficulty for mining blocks
- **Transaction Pool**: Manage transactions before they're mined into blocks
- **Merkle Tree**: Verify transaction integrity using merkle roots
- **Chain Validation**: Comprehensive blockchain verification system

### 🖥️ Interactive Frontend
- **Blockchain Overview**: View complete chain statistics and all blocks
- **Block Details**: Examine individual blocks and their transactions
- **Transaction Manager**: Add, edit, and remove transactions from the pool
- **Verification Panel**: Verify blockchain integrity with educational explanations
- **Tampering Demo**: Educational tool to show how blockchain detects unauthorized changes

### 🚀 REST API
- `GET /api/blockchain` - Get the entire blockchain
- `GET /api/block/:index` - Get a specific block and its transactions
- `GET /api/transactions` - Get all transactions in the pool
- `POST /api/transaction` - Add a new transaction
- `PUT /api/transaction/:hash` - Edit an existing transaction
- `DELETE /api/transaction/:hash` - Remove a transaction
- `GET /api/verify` - Verify blockchain integrity
- `POST /api/initialize` - Initialize blockchain with custom parameters
- `POST /api/tamper` - Tamper with a block (for educational purposes)

## Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                     Electron Frontend                          │
│  ┌─────────────────┐ ┌─────────────────┐ ┌─────────────────┐  │
│  │   Overview      │ │  Block Details  │ │  Transactions   │  │
│  │   Component     │ │    Component    │ │    Manager      │  │
│  └─────────────────┘ └─────────────────┘ └─────────────────┘  │
│  ┌─────────────────┐ ┌─────────────────┐ ┌─────────────────┐  │
│  │  Verification   │ │   HTTP Client   │ │   Main Process  │  │
│  │    Panel        │ │     (Fetch)     │ │   (IPC Bridge)  │  │
│  └─────────────────┘ └─────────────────┘ └─────────────────┘  │
└─────────────────────────────────────────────────────────────────┘
                                   │
                                   │ HTTP/REST API
                                   │
┌─────────────────────────────────────────────────────────────────┐
│                      C++ Backend                               │
│  ┌─────────────────┐ ┌─────────────────┐ ┌─────────────────┐  │
│  │   HTTP Server   │ │   Blockchain    │ │   Transaction   │  │
│  │    (httplib)    │ │     Chain       │ │      Pool       │  │
│  └─────────────────┘ └─────────────────┘ └─────────────────┘  │
│  ┌─────────────────┐ ┌─────────────────┐ ┌─────────────────┐  │
│  │     Block       │ │   Transaction   │ │     Mining      │  │
│  │     Class       │ │     Class       │ │   (Proof of     │  │
│  │                 │ │                 │ │     Work)       │  │
│  └─────────────────┘ └─────────────────┘ └─────────────────┘  │
└─────────────────────────────────────────────────────────────────┘
```

## Prerequisites

### For C++ Backend
- CMake 3.10 or higher
- C++17 compatible compiler (GCC 7+, Clang 5+)
- OpenSSL development libraries
- nlohmann/json library

### For Electron Frontend
- Node.js 14 or higher
- npm or yarn

## Installation

### 1. Install System Dependencies (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install -y build-essential cmake libssl-dev nlohmann-json3-dev
```

### 2. Build C++ Backend

```bash
cd SimpleBlockchain
mkdir -p build
cd build
cmake ..
make
```

### 3. Install Frontend Dependencies

```bash
cd desktop-electron
npm install
```

## Usage

### Running the Complete Application

1. **Start the Electron Application:**
   ```bash
   cd desktop-electron
   npm run dev
   ```
   
   The Electron app will automatically start the C++ backend server and connect to it.

### Running Backend Separately (for API testing)

```bash
cd SimpleBlockchain/build
./simpleblockchain --port 3001 --difficulty 3 --blocksize 5
```

### Command Line Options

- `--port <port>`: Set the HTTP server port (default: 3000)
- `--difficulty <num>`: Set mining difficulty (default: 3)
- `--blocksize <num>`: Set maximum transactions per block (default: 5)

## API Examples

### Add a Transaction
```bash
curl -X POST http://localhost:3001/api/transaction \
  -H "Content-Type: application/json" \
  -d '{"sender":"Alice","receiver":"Bob","amount":10.5}'
```

### Get Blockchain
```bash
curl http://localhost:3001/api/blockchain
```

### Verify Blockchain
```bash
curl http://localhost:3001/api/verify
```

### Get Specific Block
```bash
curl http://localhost:3001/api/block/0
```

## Educational Features

### 1. Blockchain Concepts Demonstrated
- **Cryptographic Hashing**: SHA-256 for block and transaction hashes
- **Merkle Trees**: Transaction integrity verification
- **Proof of Work**: Mining with adjustable difficulty
- **Chain Integrity**: Previous hash linking for immutability
- **Digital Signatures**: Transaction authenticity (simplified)

### 2. Interactive Learning
- **Real-time Verification**: See how changes affect blockchain validity
- **Tampering Demo**: Modify blocks to understand immutability
- **Visual Block Explorer**: Examine block structure and transactions
- **Transaction Lifecycle**: From pool to block confirmation

### 3. Security Demonstrations
- **Hash Chain Verification**: Detect unauthorized modifications
- **Merkle Root Validation**: Ensure transaction integrity
- **Difficulty Adjustment**: Understand mining economics
- **Double Spending Prevention**: Transaction pool management

## Development

### Project Structure
```
├── SimpleBlockchain/          # C++ Backend
│   ├── src/                  # Source files
│   ├── include/              # Header files
│   ├── build/                # Build directory
│   └── CMakeLists.txt        # Build configuration
├── desktop-electron/         # Electron Frontend
│   ├── src/
│   │   ├── main/            # Main process
│   │   ├── preload/         # Preload scripts
│   │   └── renderer/        # Renderer process (React)
│   └── package.json
└── README.md
```

### Adding New Features

1. **Backend API Endpoints**: Add routes in `SimpleBlockchain.cpp`
2. **Frontend Components**: Create React components in `src/renderer/src/components/`
3. **API Integration**: Extend the API client in `src/preload/index.js`

## Building for Production

### Package Electron App
```bash
cd desktop-electron
npm run build
npm run build:linux  # or build:win, build:mac
```

### C++ Optimization
```bash
cd SimpleBlockchain/build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## Troubleshooting

### Common Issues

1. **Cannot start C++ process**
   - Ensure the executable is built: `cd SimpleBlockchain/build && make`
   - Check file permissions: `chmod +x simpleblockchain`

2. **API connection failed**
   - Verify the server is running on the correct port
   - Check firewall settings
   - Ensure dependencies are installed

3. **Build errors**
   - Install missing dependencies
   - Check compiler version compatibility
   - Clear build cache: `rm -rf build && mkdir build`

### Debug Mode

Start the backend with debug output:
```bash
./simpleblockchain --port 3001 2>&1 | tee debug.log
```

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests if applicable
5. Submit a pull request

## Educational Use

This blockchain implementation is designed for educational purposes and includes:
- Simplified cryptography (for learning, not production security)
- Interactive tamper detection demonstrations
- Visual block and transaction exploration
- Real-time blockchain verification

**Note**: This is an educational tool and should not be used for production cryptocurrency or financial applications without significant security enhancements.