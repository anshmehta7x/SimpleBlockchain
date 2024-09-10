#pragma once
#include <vector>
#include <thread>
#include <chrono>
#include "Block.h"
#include "Transaction.h"
#include "Hash.h"
#include "TransactionPool.h"

class Chain {
private:
    std::vector<Block> blockchain;
    unsigned int difficulty;
    TransactionPool txPool;
    std::thread miningThread;
    bool stopMining;

    void miningLoop() {
        try {
            while (!stopMining) {
                if (txPool.isReadyToMine()) {
                    std::vector<Transaction> transactions = txPool.getAndClearTransactions();
                    if (!transactions.empty()) {
                        addBlock(transactions);
                        std::cout << "New block mined and added to the chain. Transactions in block: " << transactions.size() << "\n";
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            // Mine remaining transactions
            mineRemainingTransactions();
        }
        catch (const std::exception& e) {
            std::cerr << "Error in miningLoop: " << e.what() << std::endl;
            stopMining = true;
        }
    }

    void mineRemainingTransactions() {
        std::vector<Transaction> remainingTransactions = txPool.getAndClearTransactions();
        if (!remainingTransactions.empty()) {
            addBlock(remainingTransactions);
            std::cout << "Final block mined with remaining transactions. Transactions in block: " << remainingTransactions.size() << "\n";
        }
    }


public:
    Chain(unsigned int diff = 3) : difficulty(diff), stopMining(false) {
        // Create genesis block
        std::vector<Transaction> genesisTransactions;
        genesisTransactions.push_back(Transaction("Genesis", "Genesis", 0));
        Block genesisBlock(0, "0", genesisTransactions, difficulty);
        genesisBlock.mineBlock();
        blockchain.push_back(genesisBlock);

        // Start mining thread
        miningThread = std::thread(&Chain::miningLoop, this);
    }

    ~Chain() {
        stopMining = true;
        if (miningThread.joinable()) {
            miningThread.join();
        }
    }

    void addTransaction(const Transaction& tx) {
        txPool.addTransaction(tx);
        std::cout << "Transaction added to pool. Current pool size: " << txPool.size() << "\n";
    }

    void addBlock(std::vector<Transaction>& transactions) {
        Block newBlock(blockchain.size(), blockchain.back().getHash(), transactions, difficulty);
        newBlock.mineBlock();
        blockchain.push_back(newBlock);
    }

    bool verifyChain() const {
        for (size_t i = 1; i < blockchain.size(); i++) {
            const Block& currentBlock = blockchain[i];
            const Block& previousBlock = blockchain[i - 1];

            if (!currentBlock.verifyBlock()) {
                return false;
            }

            if (currentBlock.getPrevHash() != previousBlock.getHash()) {
                return false;
            }

            if (!currentBlock.verifyMerkleRoot(currentBlock.getTransactions())) {
                return false;
            }
        }
        return true;
    }

    void displayChain() const {
        for (const auto& block : blockchain) {
            block.displayBlock();
            std::cout << "\n";
        }
    }

    size_t getChainSize() const {
        return blockchain.size();
    }

    const Block& getLatestBlock() const {
        return blockchain.back  ();
    }

    //for testing verification 
    void modifyBlock(int index) {
        blockchain[index].randomModificationForTesting();
    }
};