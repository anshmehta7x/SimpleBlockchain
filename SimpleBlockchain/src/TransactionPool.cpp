#include "TransactionPool.h"

// Constructor
TransactionPool::TransactionPool(unsigned int threshold) : transactionThreshold(threshold) {}

// Set the transaction threshold
void TransactionPool::setThreshold(unsigned int threshold)
{
    this->transactionThreshold = threshold;
}

// Add a transaction to the pool
void TransactionPool::addTransaction(const Transaction &tx)
{
    std::lock_guard<std::mutex> lock(poolMutex);
    pool.push_back(tx);
}

// Check if the pool is ready for mining
bool TransactionPool::isReadyToMine() const
{
    return pool.size() >= transactionThreshold;
}

// Get and clear the transactions when ready to mine
std::vector<Transaction> TransactionPool::getAndClearTransactions()
{
    std::lock_guard<std::mutex> lock(poolMutex);
    std::vector<Transaction> transactions;
    if (pool.size() >= transactionThreshold)
    {
        transactions.assign(pool.begin(), pool.begin() + transactionThreshold);
        pool.erase(pool.begin(), pool.begin() + transactionThreshold);
    }
    return transactions;
}

// Get the size of the transaction pool
size_t TransactionPool::size() const
{
    return pool.size();
}

// Remove a transaction from the pool by its hash
bool TransactionPool::removeTransaction(const std::string &txHash)
{
    std::lock_guard<std::mutex> lock(poolMutex);
    auto it = std::find_if(pool.begin(), pool.end(),
                           [&txHash](const Transaction &tx)
                           {
                               return tx.getTxHash() == txHash;
                           });

    if (it != pool.end())
    {
        pool.erase(it);
        return true;
    }
    return false;
}

// Edit a transaction in the pool
bool TransactionPool::editTransaction(const std::string &txHash, const std::string &sender,
                                      const std::string &receiver, double amount)
{
    std::lock_guard<std::mutex> lock(poolMutex);
    auto it = std::find_if(pool.begin(), pool.end(),
                           [&txHash](const Transaction &tx)
                           {
                               return tx.getTxHash() == txHash;
                           });

    if (it != pool.end())
    {
        // Create a new transaction with updated values but preserve the original timestamp
        time_t originalTime = it->getTime();
        Transaction newTx(sender, receiver, amount, originalTime);
        // Replace the old transaction
        *it = newTx;
        return true;
    }
    return false;
}

// Display all transactions in the pool
void TransactionPool::displayTransactions() const
{
    std::lock_guard<std::mutex> lock(*const_cast<std::mutex *>(&poolMutex));

    if (pool.empty())
    {
        std::cout << "Transaction pool is empty.\n";
        return;
    }

    std::cout << "Transactions in pool (" << pool.size() << "):\n";
    for (const auto &tx : pool)
    {
        std::cout << "-----------------------------------\n";
        std::cout << "Hash: " << tx.getTxHash() << "\n";
        std::cout << "Sender: " << tx.getSender() << "\n";
        std::cout << "Receiver: " << tx.getReceiver() << "\n";
        std::cout << "Amount: " << tx.getAmount() << "\n";
        std::cout << "Timestamp: " << tx.getTime() << "\n";
    }
    std::cout << "-----------------------------------\n";
}

// Get all transactions without removing them
std::vector<Transaction> TransactionPool::getAllTransactions() const
{
    std::lock_guard<std::mutex> lock(*const_cast<std::mutex *>(&poolMutex));
    return pool;
}