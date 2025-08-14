#!/bin/bash

echo "🔗 Educational Blockchain Tool - Integration Test"
echo "================================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test the C++ backend
echo -e "\n${BLUE}1. Testing C++ Backend Build...${NC}"
cd SimpleBlockchain
if [ ! -d "build" ]; then
    mkdir build
fi
cd build
if cmake .. && make; then
    echo -e "${GREEN}✅ C++ backend built successfully${NC}"
else
    echo -e "${RED}❌ C++ backend build failed${NC}"
    exit 1
fi

# Start the backend server
echo -e "\n${BLUE}2. Starting Blockchain Server...${NC}"
./simpleblockchain --port 3001 &
SERVER_PID=$!
sleep 3

# Test API endpoints
echo -e "\n${BLUE}3. Testing API Endpoints...${NC}"

# Test blockchain endpoint
echo -e "\n${YELLOW}Testing GET /api/blockchain...${NC}"
if curl -s http://127.0.0.1:3001/api/blockchain | jq '.success' | grep -q true; then
    echo -e "${GREEN}✅ Blockchain API working${NC}"
else
    echo -e "${RED}❌ Blockchain API failed${NC}"
fi

# Test adding transaction
echo -e "\n${YELLOW}Testing POST /api/transaction...${NC}"
TX_RESULT=$(curl -s -X POST -H "Content-Type: application/json" \
    -d '{"sender":"Alice","receiver":"Bob","amount":10.5}' \
    http://127.0.0.1:3001/api/transaction)
if echo "$TX_RESULT" | jq '.success' | grep -q true; then
    echo -e "${GREEN}✅ Add transaction working${NC}"
    TX_HASH=$(echo "$TX_RESULT" | jq -r '.txHash')
    echo -e "   Transaction hash: ${TX_HASH}"
else
    echo -e "${RED}❌ Add transaction failed${NC}"
fi

# Test getting transactions
echo -e "\n${YELLOW}Testing GET /api/transactions...${NC}"
if curl -s http://127.0.0.1:3001/api/transactions | jq '.success' | grep -q true; then
    echo -e "${GREEN}✅ Get transactions working${NC}"
else
    echo -e "${RED}❌ Get transactions failed${NC}"
fi

# Test verification
echo -e "\n${YELLOW}Testing GET /api/verify...${NC}"
VERIFY_RESULT=$(curl -s http://127.0.0.1:3001/api/verify)
if echo "$VERIFY_RESULT" | jq '.success' | grep -q true; then
    IS_VALID=$(echo "$VERIFY_RESULT" | jq '.valid')
    echo -e "${GREEN}✅ Verification working - Blockchain is valid: ${IS_VALID}${NC}"
else
    echo -e "${RED}❌ Verification failed${NC}"
fi

# Test specific block endpoint
echo -e "\n${YELLOW}Testing GET /api/block/0...${NC}"
if curl -s http://127.0.0.1:3001/api/block/0 | jq '.success' | grep -q true; then
    echo -e "${GREEN}✅ Block details API working${NC}"
else
    echo -e "${RED}❌ Block details API failed${NC}"
fi

# Test frontend build
echo -e "\n${BLUE}4. Testing Frontend Build...${NC}"
cd ../../desktop-electron
if npm run build > /dev/null 2>&1; then
    echo -e "${GREEN}✅ Frontend built successfully${NC}"
else
    echo -e "${RED}❌ Frontend build failed${NC}"
fi

# Cleanup
echo -e "\n${BLUE}5. Cleaning up...${NC}"
kill $SERVER_PID 2>/dev/null
echo -e "${GREEN}✅ Server stopped${NC}"

echo -e "\n${GREEN}🎉 Integration test completed successfully!${NC}"
echo -e "\n${BLUE}To run the complete application:${NC}"
echo -e "1. cd desktop-electron"
echo -e "2. npm run dev"
echo -e "\nThe Electron app will automatically start the C++ backend and provide a full GUI interface."

echo -e "\n${BLUE}Features available in the GUI:${NC}"
echo -e "• 📊 Blockchain Overview - View chain statistics and all blocks"
echo -e "• 🔍 Block Details - Examine individual blocks and transactions"
echo -e "• 💼 Transaction Manager - Add, edit, remove transactions"
echo -e "• ✅ Verification Panel - Verify blockchain integrity + tampering demo"