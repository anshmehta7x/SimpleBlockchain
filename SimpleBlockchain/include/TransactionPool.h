#pragma once
#include <vector>
#include <mutex>
#include "Transaction.h"
#include <algorithm>
#include <iostream>

class TransactionPool
{
private:
    std::vector<Transaction> pool;
    mutable std::mutex poolMutex; // Make mutex mutable to allow locking in const methods
    unsigned int transactionThreshold;

public:
    // Constructor
    TransactionPool(unsigned int threshold = 8);

    // Set the transaction threshold
    void setThreshold(unsigned int threshold);

    // Add a transaction to the pool
    void addTransaction(const Transaction &tx);

    // Check if the pool is ready for mining
    bool isReadyToMine() const;

    // Get and clear the transactions when ready to mine
    std::vector<Transaction> getAndClearTransactions();

    // Get the size of the transaction pool
    size_t size() const;

    // Remove a transaction from the pool by its hash
    bool removeTransaction(const std::string &txHash);

    // Edit a transaction in the pool
    bool editTransaction(const std::string &txHash, const std::string &sender,
                         const std::string &receiver, double amount);

    // Display all transactions in the pool
    void displayTransactions() const;

    // Get all transactions without removing them
    std::vector<Transaction> getAllTransactions() const;
};