#pragma once
#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include "Chain.h"
#include "Transaction.h"

using json = nlohmann::json;

class BlockchainAPI
{
private:
    std::unique_ptr<Chain> blockchain;

public:
    // Initialize the blockchain with given difficulty and block size
    BlockchainAPI(unsigned int difficulty = 3, unsigned int blockSize = 5)
    {
        blockchain = std::make_unique<Chain>(difficulty, blockSize);
    }

    // Add a new transaction to the pool
    json addTransaction(const std::string &sender, const std::string &receiver, double amount)
    {
        json response;
        try
        {
            Transaction tx(sender, receiver, amount);
            blockchain->addTransaction(tx);

            response["success"] = true;
            response["message"] = "Transaction added successfully";
            response["txHash"] = tx.getTxHash();
            response["sender"] = tx.getSender();
            response["receiver"] = tx.getReceiver();
            response["amount"] = tx.getAmount();
            response["timestamp"] = tx.getTime();
        }
        catch (const std::exception &e)
        {
            response["success"] = false;
            response["message"] = std::string("Error: ") + e.what();
        }
        return response;
    }

    // Remove a transaction from the pool
    json removeTransaction(const std::string &txHash)
    {
        json response;
        bool success = blockchain->removeTransactionFromPool(txHash);

        if (success)
        {
            response["success"] = true;
            response["message"] = "Transaction removed successfully";
        }
        else
        {
            response["success"] = false;
            response["message"] = "Transaction not found in pool";
        }
        return response;
    }

    // Edit a transaction in the pool
    json editTransaction(const std::string &txHash, const std::string &sender,
                         const std::string &receiver, double amount)
    {
        json response;
        bool success = blockchain->editTransactionInPool(txHash, sender, receiver, amount);

        if (success)
        {
            response["success"] = true;
            response["message"] = "Transaction edited successfully";
        }
        else
        {
            response["success"] = false;
            response["message"] = "Transaction not found in pool";
        }
        return response;
    }

    // Get all transactions in the pool
    json getTransactionPool()
    {
        json response;
        std::vector<Transaction> transactions = blockchain->getPoolTransactions();

        json txArray = json::array();
        for (const auto &tx : transactions)
        {
            json txObj;
            txObj["hash"] = tx.getTxHash();
            txObj["sender"] = tx.getSender();
            txObj["receiver"] = tx.getReceiver();
            txObj["amount"] = tx.getAmount();
            txObj["timestamp"] = tx.getTime();
            txArray.push_back(txObj);
        }

        response["success"] = true;
        response["transactions"] = txArray;
        response["count"] = transactions.size();
        return response;
    }

    // Verify the blockchain
    json verifyBlockchain()
    {
        json response;
        bool isValid = blockchain->verifyChain();

        response["success"] = true;
        response["valid"] = isValid;
        response["message"] = isValid ? "Blockchain is valid" : "Blockchain verification failed";
        return response;
    }

    // Get the entire blockchain
    json getBlockchain()
    {
        json response;
        response["success"] = true;
        response["chainSize"] = blockchain->getChainSize();

        // Get blocks from the chain
        json blocksArray = json::array();
        for (size_t i = 0; i < blockchain->getChainSize(); i++)
        {
            const Block &block = blockchain->getBlockAt(i);

            json blockObj;
            blockObj["id"] = i;
            blockObj["hash"] = block.getHash();
            blockObj["prevHash"] = block.getPrevHash();
            blockObj["merkleRoot"] = block.getMerkleRoot();
            blockObj["nonce"] = block.getNonce();
            blockObj["timestamp"] = block.getTime();

            json txArray = json::array();
            for (const auto &tx : block.getTransactions())
            {
                json txObj;
                txObj["hash"] = tx.getTxHash();
                txObj["sender"] = tx.getSender();
                txObj["receiver"] = tx.getReceiver();
                txObj["amount"] = tx.getAmount();
                txObj["timestamp"] = tx.getTime();
                txArray.push_back(txObj);
            }

            blockObj["transactions"] = txArray;
            blockObj["transactionCount"] = txArray.size();
            blocksArray.push_back(blockObj);
        }

        response["blocks"] = blocksArray;
        return response;
    }

    // Save blockchain to file
    json saveBlockchain(const std::string &filename)
    {
        json response;
        bool success = blockchain->writeCurrentChainToFile(filename);

        if (success)
        {
            response["success"] = true;
            response["message"] = "Blockchain saved to " + filename;
        }
        else
        {
            response["success"] = false;
            response["message"] = "Failed to save blockchain";
        }
        return response;
    }

    // Load blockchain from file
    json loadBlockchain(const std::string &filename)
    {
        json response;
        bool success = blockchain->readChainFromFile(filename);

        if (success)
        {
            response["success"] = true;
            response["message"] = "Blockchain loaded from " + filename;
            response["chainSize"] = blockchain->getChainSize();
        }
        else
        {
            response["success"] = false;
            response["message"] = "Failed to load blockchain";
        }
        return response;
    }

    // Tamper with a block (for demonstration purposes)
    json tamperWithBlock(size_t blockIndex)
    {
        json response;

        if (blockIndex >= blockchain->getChainSize())
        {
            response["success"] = false;
            response["message"] = "Invalid block index";
            return response;
        }

        blockchain->modifyBlock(blockIndex);

        response["success"] = true;
        response["message"] = "Block " + std::to_string(blockIndex) + " has been tampered with";
        return response;
    }
};