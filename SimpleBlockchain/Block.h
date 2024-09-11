#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include "Transaction.h"
#include "Mine.h"

class Block
{
private:
    unsigned int difficulty;
    unsigned int blockId;
    std::string prevHash;
    std::string blockHash;
    std::string merkleRoot;
    std::vector<Transaction> transactions;
    unsigned int nonce;
    time_t timestamp;
    bool minedStatus;

public:
    Block() : difficulty(0), blockId(0), nonce(0), timestamp(0), minedStatus(false) {}

    Block(unsigned int id, std::string prevHash, std::vector<Transaction> txs, unsigned int difficulty)
        : blockId(id), prevHash(prevHash), transactions(txs), difficulty(difficulty),
        nonce(0), timestamp(time(0)), minedStatus(false), blockHash("") {
        this->merkleRoot = calculateMerkleRoot(this->transactions);
    }

    std::string calculateMerkleRoot(const std::vector<Transaction>& txs) const {
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

    bool verifyMerkleRoot(const std::vector<Transaction> txs) const {
        std::string testMerkleRoot = calculateMerkleRoot(txs);
        return testMerkleRoot == this->merkleRoot;
    }

    void mineBlock() {
        if (minedStatus) {
            return;
        }
        std::pair<std::string, int> result = proofOfWork(prevHash, this->merkleRoot, this->difficulty, false);
        this->blockHash = result.first;
        this->nonce = result.second;
        minedStatus = true;
    }

    bool verifyBlock() const {
        std::string data = prevHash + std::to_string(nonce) + this->merkleRoot;
        return verifyHash(data, blockHash);
    }

    // Getter methods
    std::string getHash() const { return this->blockHash; }
    unsigned int getNonce() const { return this->nonce; }
    std::string getPrevHash() const { return this->prevHash; }
    time_t getTime() const { return this->timestamp; }
	std::string getMerkleRoot() const { return this->merkleRoot; }
    const std::vector<Transaction>& getTransactions() const { return this->transactions; }

    void displayBlock() const {
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

    void randomModificationForTesting() {
        transactions[0].randomModification();
        this->merkleRoot = calculateMerkleRoot(this->transactions);
    }
};