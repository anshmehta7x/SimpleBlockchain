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
    TransactionPool(unsigned int threshold = 8) : transactionThreshold(threshold) {}

    void addTransaction(const Transaction& tx) {
        std::lock_guard<std::mutex> lock(poolMutex);
        pool.push_back(tx);
    }

    bool isReadyToMine() const {
        return pool.size() >= transactionThreshold;
    }

    std::vector<Transaction> getAndClearTransactions() {
        std::lock_guard<std::mutex> lock(poolMutex);
        std::vector<Transaction> transactions;
        if (pool.size() >= transactionThreshold) {
            transactions.assign(pool.begin(), pool.begin() + transactionThreshold);
            pool.erase(pool.begin(), pool.begin() + transactionThreshold);
        }
        return transactions;
    }

    size_t size() const {
        return pool.size();
    }
};
