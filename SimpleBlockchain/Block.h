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
    // Constructors
    Block();
    Block(unsigned int id, std::string prevHash, std::vector<Transaction> txs, unsigned int difficulty);

    // Merkle root calculation and verification
    std::string calculateMerkleRoot(const std::vector<Transaction>& txs) const;
    bool verifyMerkleRoot(const std::vector<Transaction> txs) const;

    // Mining and verification functions
    void mineBlock();
    bool verifyBlock() const;

    // Getter methods
    std::string getHash() const;
    unsigned int getNonce() const;
    std::string getPrevHash() const;
    time_t getTime() const;
    std::string getMerkleRoot() const;
    const std::vector<Transaction>& getTransactions() const;

    // Display block details
    void displayBlock() const;

    // Function for testing block modification
    void randomModificationForTesting();
};
