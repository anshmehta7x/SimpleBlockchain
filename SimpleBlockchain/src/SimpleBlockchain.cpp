#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <memory>
#include <cstring>
#include <sstream>
#include <functional>
#include <unordered_map>
#include <mutex>
#include <condition_variable>

// Include your blockchain headers
#include "Transaction.h"
#include "Hash.h"
#include "Mine.h"
#include "Block.h"
#include "Utilities.h"
#include "Chain.h"
#include "TransactionPool.h"

// Include HTTP server libraries
#include "httplib.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Global blockchain instance
std::unique_ptr<Chain> blockchain;
std::mutex chainMutex;

// Helper function to parse JSON request body
json parseRequestBody(const httplib::Request &req)
{
    try
    {
        return json::parse(req.body);
    }
    catch (const std::exception &e)
    {
        return json::object();
    }
}

int main(int argc, char *argv[])
{
    std::cout << "Blockchain HTTP Server Starting...\n";

    // Default values for difficulty and block size
    unsigned int difficulty = 3;
    unsigned int blockSize = 5;
    int port = 3000;

    // Parse command line arguments if provided
    for (int i = 1; i < argc; i++)
    {
        if (std::string(argv[i]) == "--difficulty" && i + 1 < argc)
        {
            difficulty = std::stoi(argv[i + 1]);
            i++;
        }
        else if (std::string(argv[i]) == "--blocksize" && i + 1 < argc)
        {
            blockSize = std::stoi(argv[i + 1]);
            i++;
        }
        else if (std::string(argv[i]) == "--port" && i + 1 < argc)
        {
            port = std::stoi(argv[i + 1]);
            i++;
        }
    }

    // Initialize blockchain
    blockchain = std::make_unique<Chain>(difficulty, blockSize);
    std::cout << "Blockchain initialized with difficulty " << difficulty
              << " and block size " << blockSize << std::endl;

    // Create HTTP server
    httplib::Server server;

    // CORS middleware for development
    server.set_base_dir("./public");
    server.set_default_headers({{"Access-Control-Allow-Origin", "*"},
                                {"Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS"},
                                {"Access-Control-Allow-Headers", "Content-Type, Authorization"}});

    // Handle OPTIONS requests for CORS
    server.Options("/(.*)", [](const httplib::Request &, httplib::Response &res)
                   {
                       res.status = 204; // No content
                   });

    // Initialize blockchain with custom parameters
    server.Post("/api/initialize", [](const httplib::Request &req, httplib::Response &res)
                {
        auto body = parseRequestBody(req);
        json response;
        
        try {
            if (body.contains("difficulty") && body.contains("blockSize")) {
                unsigned int difficulty = body["difficulty"].get<unsigned int>();
                unsigned int blockSize = body["blockSize"].get<unsigned int>();
                
                std::lock_guard<std::mutex> lock(chainMutex);
                blockchain = std::make_unique<Chain>(difficulty, blockSize);
                
                response["success"] = true;
                response["message"] = "Blockchain initialized successfully";
                response["difficulty"] = difficulty;
                response["blockSize"] = blockSize;
            } else {
                response["success"] = false;
                response["message"] = "Missing required parameters: difficulty and blockSize";
            }
        } catch (const std::exception& e) {
            response["success"] = false;
            response["message"] = std::string("Error: ") + e.what();
        }
        
        res.set_content(response.dump(), "application/json"); });

    // Add a transaction
    server.Post("/api/transaction", [](const httplib::Request &req, httplib::Response &res)
                {
        auto body = parseRequestBody(req);
        json response;
        
        try {
            if (body.contains("sender") && body.contains("receiver") && body.contains("amount")) {
                std::string sender = body["sender"].get<std::string>();
                std::string receiver = body["receiver"].get<std::string>();
                double amount = body["amount"].get<double>();
                
                Transaction tx(sender, receiver, amount);
                
                std::lock_guard<std::mutex> lock(chainMutex);
                blockchain->addTransaction(tx);
                
                response["success"] = true;
                response["message"] = "Transaction added successfully";
                response["txHash"] = tx.getTxHash();
                response["sender"] = tx.getSender();
                response["receiver"] = tx.getReceiver();
                response["amount"] = tx.getAmount();
                response["timestamp"] = tx.getTime();
            } else {
                response["success"] = false;
                response["message"] = "Missing required parameters: sender, receiver, amount";
            }
        } catch (const std::exception& e) {
            response["success"] = false;
            response["message"] = std::string("Error: ") + e.what();
        }
        
        res.set_content(response.dump(), "application/json"); });

    // Remove a transaction
    server.Delete("/api/transaction/:hash", [](const httplib::Request &req, httplib::Response &res)
                  {
        json response;
        
        try {
            std::string txHash = req.path_params.at("hash");
            
            std::lock_guard<std::mutex> lock(chainMutex);
            bool success = blockchain->removeTransactionFromPool(txHash);
            
            if (success) {
                response["success"] = true;
                response["message"] = "Transaction removed successfully";
            } else {
                response["success"] = false;
                response["message"] = "Transaction not found in pool";
            }
        } catch (const std::exception& e) {
            response["success"] = false;
            response["message"] = std::string("Error: ") + e.what();
        }
        
        res.set_content(response.dump(), "application/json"); });

    // Edit a transaction
    server.Put("/api/transaction/:hash", [](const httplib::Request &req, httplib::Response &res)
               {
        auto body = parseRequestBody(req);
        json response;
        
        try {
            std::string txHash = req.path_params.at("hash");
            
            if (body.contains("sender") && body.contains("receiver") && body.contains("amount")) {
                std::string sender = body["sender"].get<std::string>();
                std::string receiver = body["receiver"].get<std::string>();
                double amount = body["amount"].get<double>();
                
                std::lock_guard<std::mutex> lock(chainMutex);
                bool success = blockchain->editTransactionInPool(txHash, sender, receiver, amount);
                
                if (success) {
                    response["success"] = true;
                    response["message"] = "Transaction edited successfully";
                } else {
                    response["success"] = false;
                    response["message"] = "Transaction not found in pool";
                }
            } else {
                response["success"] = false;
                response["message"] = "Missing required parameters: sender, receiver, amount";
            }
        } catch (const std::exception& e) {
            response["success"] = false;
            response["message"] = std::string("Error: ") + e.what();
        }
        
        res.set_content(response.dump(), "application/json"); });

    // Get transaction pool
    server.Get("/api/transactions", [](const httplib::Request &, httplib::Response &res)
               {
        json response;
        
        try {
            std::lock_guard<std::mutex> lock(chainMutex);
            std::vector<Transaction> transactions = blockchain->getPoolTransactions();
            
            json txArray = json::array();
            for (const auto& tx : transactions) {
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
        } catch (const std::exception& e) {
            response["success"] = false;
            response["message"] = std::string("Error: ") + e.what();
        }
        
        res.set_content(response.dump(), "application/json"); });

    // Verify blockchain
    server.Get("/api/verify", [](const httplib::Request &, httplib::Response &res)
               {
        json response;
        
        try {
            std::lock_guard<std::mutex> lock(chainMutex);
            bool isValid = blockchain->verifyChain();
            
            response["success"] = true;
            response["valid"] = isValid;
            response["message"] = isValid ? "Blockchain is valid" : "Blockchain verification failed";
        } catch (const std::exception& e) {
            response["success"] = false;
            response["message"] = std::string("Error: ") + e.what();
        }
        
        res.set_content(response.dump(), "application/json"); });

    // Get blockchain
    server.Get("/api/blockchain", [](const httplib::Request &, httplib::Response &res)
               {
        json response;
        
        try {
            std::lock_guard<std::mutex> lock(chainMutex);
            response["success"] = true;
            response["chainSize"] = blockchain->getChainSize();
            
            // Get blocks from the chain
            json blocksArray = json::array();
            for (size_t i = 0; i < blockchain->getChainSize(); i++) {
                const Block& block = blockchain->getBlockAt(i);
                
                json blockObj;
                blockObj["id"] = i;
                blockObj["hash"] = block.getHash();
                blockObj["prevHash"] = block.getPrevHash();
                blockObj["merkleRoot"] = block.getMerkleRoot();
                blockObj["nonce"] = block.getNonce();
                blockObj["timestamp"] = block.getTime();
                
                json txArray = json::array();
                for (const auto& tx : block.getTransactions()) {
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
        } catch (const std::exception& e) {
            response["success"] = false;
            response["message"] = std::string("Error: ") + e.what();
        }
        
        res.set_content(response.dump(), "application/json"); });

    // Save blockchain
    server.Post("/api/save", [](const httplib::Request &req, httplib::Response &res)
                {
        auto body = parseRequestBody(req);
        json response;
        
        try {
            if (body.contains("filename")) {
                std::string filename = body["filename"].get<std::string>();
                
                std::lock_guard<std::mutex> lock(chainMutex);
                bool success = blockchain->writeCurrentChainToFile(filename);
                
                if (success) {
                    response["success"] = true;
                    response["message"] = "Blockchain saved to " + filename;
                } else {
                    response["success"] = false;
                    response["message"] = "Failed to save blockchain";
                }
            } else {
                response["success"] = false;
                response["message"] = "Missing required parameter: filename";
            }
        } catch (const std::exception& e) {
            response["success"] = false;
            response["message"] = std::string("Error: ") + e.what();
        }
        
        res.set_content(response.dump(), "application/json"); });

    // Load blockchain
    server.Post("/api/load", [](const httplib::Request &req, httplib::Response &res)
                {
        auto body = parseRequestBody(req);
        json response;
        
        try {
            if (body.contains("filename")) {
                std::string filename = body["filename"].get<std::string>();
                
                std::lock_guard<std::mutex> lock(chainMutex);
                bool success = blockchain->readChainFromFile(filename);
                
                if (success) {
                    response["success"] = true;
                    response["message"] = "Blockchain loaded from " + filename;
                    response["chainSize"] = blockchain->getChainSize();
                } else {
                    response["success"] = false;
                    response["message"] = "Failed to load blockchain";
                }
            } else {
                response["success"] = false;
                response["message"] = "Missing required parameter: filename";
            }
        } catch (const std::exception& e) {
            response["success"] = false;
            response["message"] = std::string("Error: ") + e.what();
        }
        
        res.set_content(response.dump(), "application/json"); });

    // Tamper with block (for demonstration)
    server.Post("/api/tamper", [](const httplib::Request &req, httplib::Response &res)
                {
        auto body = parseRequestBody(req);
        json response;
        
        try {
            if (body.contains("blockIndex")) {
                size_t blockIndex = body["blockIndex"].get<size_t>();
                
                std::lock_guard<std::mutex> lock(chainMutex);
                if (blockIndex >= blockchain->getChainSize()) {
                    response["success"] = false;
                    response["message"] = "Invalid block index";
                } else {
                    blockchain->modifyBlock(blockIndex);
                    response["success"] = true;
                    response["message"] = "Block " + std::to_string(blockIndex) + " has been tampered with";
                }
            } else {
                response["success"] = false;
                response["message"] = "Missing required parameter: blockIndex";
            }
        } catch (const std::exception& e) {
            response["success"] = false;
            response["message"] = std::string("Error: ") + e.what();
        }
        
        res.set_content(response.dump(), "application/json"); });

    // Start server
    std::cout << "HTTP server starting on port " << port << "...\n";
    server.listen("127.0.0.1", port);

    return 0;
}