# SimpleBlockchain - Educational Blockchain Tool

A comprehensive educational blockchain application with C++ backend and Electron frontend for learning blockchain technology.

## 🏗️ Architecture

- **C++ Backend**: High-performance blockchain implementation with HTTP REST API
- **Electron Frontend**: Modern React-based GUI for interactive blockchain exploration

## 🚀 Quick Start (Automatic Build)

### Prerequisites
- Node.js (v16 or higher)
- npm or yarn
- CMake (v3.16 or higher)
- C++ compiler (GCC, Clang, or MSVC)

### Installation & Run
```bash
# Navigate to the Electron app directory
cd desktop-electron

# Install dependencies
npm install

# Build C++ backend and run the application
npm run dev
```

The `npm run dev` command will automatically:
1. Build the C++ backend using CMake
2. Start the Electron application
3. Launch the blockchain server
4. Open the GUI interface

## 🔧 Manual Build Instructions

If you want to build the components separately or the automatic build fails:

### Building the C++ Backend

#### On Linux/macOS:
```bash
# Navigate to the C++ directory
cd SimpleBlockchain

# Create build directory
mkdir -p build
cd build

# Generate build files
cmake ..

# Compile the project
make

# Run the server (optional, for testing)
./simpleblockchain --port 3001
```

#### On Windows:
```cmd
# Navigate to the C++ directory
cd SimpleBlockchain

# Create build directory
mkdir build
cd build

# Generate build files
cmake ..

# Compile the project
cmake --build . --config Release

# Run the server (optional, for testing)
simpleblockchain.exe --port 3001
```

### Building the Electron Frontend

```bash
# Navigate to the Electron app directory
cd desktop-electron

# Install dependencies
npm install

# Build the frontend (assumes C++ backend is already built)
npm run build

# Or run in development mode
npm run start
```

## 🛠️ Development Scripts

### Available npm scripts in `desktop-electron/`:

- `npm run dev` - Build C++ backend and run in development mode
- `npm run build:cpp` - Build only the C++ backend
- `npm run build:cpp:clean` - Clean build the C++ backend
- `npm run build:cpp:windows` - Build C++ backend on Windows
- `npm run build` - Build C++ backend and Electron app
- `npm run start` - Start the Electron app (requires pre-built C++ backend)

## 🐛 Troubleshooting

### Common Issues

#### 1. CMake not found
```bash
# On Ubuntu/Debian
sudo apt update && sudo apt install cmake

# On macOS with Homebrew
brew install cmake

# On Windows
# Download from https://cmake.org/download/
```

#### 2. C++ compiler not found
```bash
# On Ubuntu/Debian
sudo apt install build-essential

# On macOS
xcode-select --install

# On Windows
# Install Visual Studio with C++ tools or MinGW
```

#### 3. Build fails with missing nlohmann/json
The project includes the nlohmann/json header in the `include/` directory. If you get errors:
```bash
# Make sure you're in the SimpleBlockchain directory
cd SimpleBlockchain

# Check that include/nlohmann/json.hpp exists
ls include/nlohmann/

# If missing, you can download it:
mkdir -p include/nlohmann
curl -o include/nlohmann/json.hpp https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp
```

#### 4. Electron app can't find C++ executable
Make sure the C++ backend is built before running the Electron app:
```bash
# Build C++ backend first
cd SimpleBlockchain
mkdir -p build && cd build && cmake .. && make

# Then run Electron app
cd ../../desktop-electron
npm run start
```

#### 5. Port conflicts
If port 3001 is in use, you can specify a different port:
```bash
# In SimpleBlockchain/build/
./simpleblockchain --port 3002

# Update the Electron app to use the new port in src/main/index.js
```

## 📁 Project Structure

```
SimpleBlockchain/
├── SimpleBlockchain/              # C++ Backend
│   ├── include/                   # Header files
│   │   ├── nlohmann/             # JSON library
│   │   └── *.h                   # Blockchain headers
│   ├── src/                      # Source files
│   │   ├── SimpleBlockchain.cpp  # Main server application
│   │   └── *.cpp                # Blockchain implementation
│   ├── build/                    # Build output (created during build)
│   ├── CMakeLists.txt            # CMake build configuration
│   └── run.sh                    # Legacy build script
├── desktop-electron/             # Electron Frontend
│   ├── src/                      # Application source
│   │   ├── main/                 # Main process
│   │   ├── renderer/             # React frontend
│   │   └── preload/              # Preload scripts
│   ├── package.json              # Node.js dependencies & scripts
│   └── electron.vite.config.mjs  # Build configuration
└── README.md                     # This file
```

## 🌐 API Endpoints

The C++ backend provides these REST API endpoints:

- `GET /api/blockchain` - Get complete blockchain
- `GET /api/block/:index` - Get specific block details
- `GET /api/transactions` - Get transaction pool
- `POST /api/transaction` - Add new transaction
- `PUT /api/transaction/:hash` - Edit existing transaction
- `DELETE /api/transaction/:hash` - Remove transaction
- `GET /api/verify` - Verify blockchain integrity
- `POST /api/initialize` - Initialize blockchain with custom parameters
- `POST /api/tamper` - Tamper with block (educational)

## 🎓 Educational Features

- **Interactive Block Explorer**: Visual representation of the entire blockchain
- **Transaction Lifecycle**: Watch transactions move from pool to blocks
- **Tampering Demonstration**: Educational tool showing how blockchain detects unauthorized changes
- **Real-time Verification**: See how modifications affect blockchain validity
- **Hash Visualization**: Display and explain cryptographic hashes and merkle roots

## 🤝 Contributing

This is an educational project. Feel free to:
- Report issues with build instructions
- Suggest improvements to the build process
- Add new educational features
- Improve documentation

## 📝 License

This project is for educational purposes. See the license file for details.