#pragma once
#include "Block.h"
#include <fstream>
#include "Utilities.h"

// Constructor definitions
Block::Block() : difficulty(0), blockId(0), nonce(0), timestamp(0), minedStatus(false) {}

Block::Block(unsigned int id, std::string prevHash, std::vector<Transaction> txs, unsigned int difficulty)
    : blockId(id), prevHash(prevHash), transactions(txs), difficulty(difficulty),
    nonce(0), timestamp(time(0)), minedStatus(false), blockHash("") {
    this->merkleRoot = calculateMerkleRoot(this->transactions);
}

// Calculate the Merkle root for the block's transactions
std::string Block::calculateMerkleRoot(const std::vector<Transaction>& txs) const {
    if (txs.empty()) {
        return "";
    }
    std::vector<std::string> merkleTree;
    for (const Transaction& t : txs) {
        merkleTree.push_back(t.getTxHash());
    }
    while (merkleTree.size() > 1) {
        std::vector<std::string> newLevel;
        for (size_t i = 0; i < merkleTree.size(); i += 2) {
            if (i + 1 == merkleTree.size()) {
                newLevel.push_back(sha256_block(merkleTree[i] + merkleTree[i]));
            }
            else {
                newLevel.push_back(sha256_block(merkleTree[i] + merkleTree[i + 1]));
            }
        }
        merkleTree = newLevel;
    }
    return merkleTree[0];
}

// Verify if the calculated Merkle root matches the stored one
bool Block::verifyMerkleRoot(const std::vector<Transaction> txs) const {
    std::string testMerkleRoot = calculateMerkleRoot(txs);
    return testMerkleRoot == this->merkleRoot;
}

// Mining the block with proof of work
void Block::mineBlock() {
    if (minedStatus) {
        return;
    }
    std::pair<std::string, int> result = proofOfWork(prevHash, this->merkleRoot, this->difficulty, true);
    this->blockHash = result.first;
    this->nonce = result.second;
    minedStatus = true;
}

// Verify if the block's hash is valid
bool Block::verifyBlock() const {
    std::string data = prevHash + std::to_string(nonce) + this->merkleRoot;
    return verifyHash(data, blockHash);
}

// Getter methods
std::string Block::getHash() const { return this->blockHash; }
unsigned int Block::getNonce() const { return this->nonce; }
std::string Block::getPrevHash() const { return this->prevHash; }
time_t Block::getTime() const { return this->timestamp; }
std::string Block::getMerkleRoot() const { return this->merkleRoot; }
const std::vector<Transaction>& Block::getTransactions() const { return this->transactions; }

// Display block details
void Block::displayBlock() const {
    if (blockHash.empty()) {
        std::cout << "Block not mined yet\n";
    }
    else {
        std::cout << "_________________________________________________________________________________________\n";
        std::cout << "Block ID: " << blockId << "\n";
        std::cout << "Previous Hash: " << prevHash << "\n";
        std::cout << "Hash: " << blockHash << "\n";
        std::cout << "Nonce: " << nonce << "\n";
        std::cout << "Timestamp: " << timestamp << "\n";
        std::cout << "Merkle Root: " << merkleRoot << "\n";
        std::cout << "Transactions: \n";
        for (const Transaction& tx : transactions) {
            std::cout << "-----------------------------------\n";
            std::cout << "Sender: " << tx.getSender() << "\n";
            std::cout << "Receiver: " << tx.getReceiver() << "\n";
            std::cout << "Amount: " << tx.getAmount() << "\n";
            std::cout << "Timestamp: " << tx.getTime() << "\n";
            std::cout << "Transaction Hash: " << tx.getTxHash() << "\n";
            std::cout << "-----------------------------------\n";
        }
        std::cout << "_________________________________________________________________________________________\n";
    }
}

// Modify the block for testing purposes
void Block::randomModificationForTesting() {
    transactions[0].randomModification();
    this->merkleRoot = calculateMerkleRoot(this->transactions);
}

// Write transactions to a file
void Block::writeTxsToFile() const {

    std::ofstream output("transactions.txt");
    if (output.is_open()) {
        for (Transaction t : this->transactions) {
            std::string line = std::to_string(t.getTime()) + "," + t.getSender() + "," + t.getReceiver() + "," + std::to_string(t.getAmount()) + "," + t.getTxHash();
            output << line << '\n';
        }
    }
    else {
        std::cerr << "Error: Could not open transactions.txt for writing." << std::endl;

    }
    output.close();
}

void Block:: readTxsFromFile() {
    std::vector<Transaction> txs;
    std::ifstream input("transactions.txt");
    if (input.is_open()) {
        std::string line = "";
        while (std::getline(input, line)) {
            std::cout << line << std::endl;
            std::vector<string> tokens = helpers::lineSplit(line, ',');
            Transaction temp(tokens[1], tokens[2], stod(tokens[3]), stoi(tokens[0]));
            temp.setTxHash();
            txs.push_back(temp);
        }
    }
    else {
        cerr << "Error: Could not open transactions.txt for reading." << endl;

    }
    this->transactions = txs;
}