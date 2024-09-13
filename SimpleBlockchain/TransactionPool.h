#pragma once
#include <vector>
#include <mutex>
#include "Transaction.h"

class TransactionPool {
private:
    std::vector<Transaction> pool;
    std::mutex poolMutex;
    unsigned int transactionThreshold;

public:
    // Constructor
    TransactionPool(unsigned int threshold = 8);

    // Set the transaction threshold
    void setThreshold(unsigned int threshold);

    // Add a transaction to the pool
    void addTransaction(const Transaction& tx);

    // Check if the pool is ready for mining
    bool isReadyToMine() const;

    // Get and clear the transactions when ready to mine
    std::vector<Transaction> getAndClearTransactions();

    // Get the size of the transaction pool
    size_t size() const;
};
