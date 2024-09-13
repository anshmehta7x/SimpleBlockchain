#include "TransactionPool.h"

// Constructor
TransactionPool::TransactionPool(unsigned int threshold) : transactionThreshold(threshold) {}

// Set the transaction threshold
void TransactionPool::setThreshold(unsigned int threshold) {
    this->transactionThreshold = threshold;
}

// Add a transaction to the pool
void TransactionPool::addTransaction(const Transaction& tx) {
    std::lock_guard<std::mutex> lock(poolMutex);
    pool.push_back(tx);
}

// Check if the pool is ready for mining
bool TransactionPool::isReadyToMine() const {
    return pool.size() >= transactionThreshold;
}

// Get and clear the transactions when ready to mine
std::vector<Transaction> TransactionPool::getAndClearTransactions() {
    std::lock_guard<std::mutex> lock(poolMutex);
    std::vector<Transaction> transactions;
    if (pool.size() >= transactionThreshold) {
        transactions.assign(pool.begin(), pool.begin() + transactionThreshold);
        pool.erase(pool.begin(), pool.begin() + transactionThreshold);
    }
    return transactions;
}

// Get the size of the transaction pool
size_t TransactionPool::size() const {
    return pool.size();
}
