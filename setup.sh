#!/bin/bash

# SimpleBlockchain Build Setup Script
# This script helps set up the development environment

echo "SimpleBlockchain - Build Setup Script"
echo "====================================="

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to install packages on different systems
install_dependencies() {
    echo "Installing dependencies..."
    
    if command_exists apt-get; then
        # Ubuntu/Debian
        echo "Detected Ubuntu/Debian system"
        sudo apt update
        sudo apt install -y cmake build-essential libssl-dev curl
    elif command_exists yum; then
        # RHEL/CentOS/Fedora
        echo "Detected RHEL/CentOS/Fedora system"
        sudo yum install -y cmake gcc-c++ openssl-devel curl
    elif command_exists brew; then
        # macOS
        echo "Detected macOS system"
        brew install cmake openssl curl
    else
        echo "Could not detect package manager. Please install manually:"
        echo "  - CMake (v3.16+)"
        echo "  - C++ compiler (GCC/Clang)"
        echo "  - OpenSSL development libraries"
        echo "  - curl"
        return 1
    fi
}

# Check for required tools
echo ""
echo "Checking for required dependencies..."

MISSING_DEPS=0

if ! command_exists cmake; then
    echo "❌ CMake not found"
    MISSING_DEPS=1
else
    CMAKE_VERSION=$(cmake --version | grep -oE '[0-9]+\.[0-9]+' | head -1)
    echo "✅ CMake found (version $CMAKE_VERSION)"
fi

if ! command_exists gcc && ! command_exists clang; then
    echo "❌ C++ compiler not found (gcc or clang)"
    MISSING_DEPS=1
else
    if command_exists gcc; then
        GCC_VERSION=$(gcc --version | grep -oE '[0-9]+\.[0-9]+' | head -1)
        echo "✅ GCC found (version $GCC_VERSION)"
    fi
    if command_exists clang; then
        CLANG_VERSION=$(clang --version | grep -oE '[0-9]+\.[0-9]+' | head -1)
        echo "✅ Clang found (version $CLANG_VERSION)"
    fi
fi

if ! command_exists node; then
    echo "❌ Node.js not found"
    MISSING_DEPS=1
else
    NODE_VERSION=$(node --version)
    echo "✅ Node.js found ($NODE_VERSION)"
fi

if ! command_exists npm; then
    echo "❌ npm not found"
    MISSING_DEPS=1
else
    NPM_VERSION=$(npm --version)
    echo "✅ npm found (version $NPM_VERSION)"
fi

# Check for OpenSSL development libraries
if ! pkg-config --exists openssl 2>/dev/null && ! ls /usr/include/openssl/ssl.h 2>/dev/null && ! ls /usr/local/include/openssl/ssl.h 2>/dev/null; then
    echo "❌ OpenSSL development libraries not found"
    MISSING_DEPS=1
else
    echo "✅ OpenSSL development libraries found"
fi

echo ""

if [ $MISSING_DEPS -eq 1 ]; then
    echo "Some dependencies are missing."
    read -p "Would you like to install them automatically? (y/n): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        install_dependencies
    else
        echo "Please install the missing dependencies manually and run this script again."
        exit 1
    fi
fi

echo ""
echo "Setting up the project..."

# Download nlohmann/json if missing
if [ ! -f "SimpleBlockchain/include/nlohmann/json.hpp" ]; then
    echo "Downloading nlohmann/json library..."
    mkdir -p SimpleBlockchain/include/nlohmann
    curl -o SimpleBlockchain/include/nlohmann/json.hpp https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp
    if [ $? -eq 0 ]; then
        echo "✅ nlohmann/json downloaded successfully"
    else
        echo "❌ Failed to download nlohmann/json"
        exit 1
    fi
else
    echo "✅ nlohmann/json already present"
fi

# Install npm dependencies
echo "Installing npm dependencies..."
cd desktop-electron
npm install
if [ $? -eq 0 ]; then
    echo "✅ npm dependencies installed"
else
    echo "❌ Failed to install npm dependencies"
    exit 1
fi

echo ""
echo "Building C++ backend..."
npm run build:cpp
if [ $? -eq 0 ]; then
    echo "✅ C++ backend built successfully"
else
    echo "❌ Failed to build C++ backend"
    exit 1
fi

echo ""
echo "🎉 Setup complete!"
echo ""
echo "To run the application:"
echo "  cd desktop-electron"
echo "  npm run dev"
echo ""
echo "To build for production:"
echo "  cd desktop-electron"
echo "  npm run build"
echo ""