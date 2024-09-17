#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <chrono>
#include <thread>
#include "Transaction.h"
#include "Hash.h"
#include "Mine.h"
#include "Block.h"
#include "Utilities.h"
#include "Chain.h"

using namespace std;

//for testing 
void populateTransactions(int count) {
    std::vector<Transaction> transactions;
    std::vector<std::string> users = { "Alice", "Bob", "Charlie", "David", "Eve","Fein"};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> userDist(0, users.size() - 1);
    std::uniform_real_distribution<> amountDist(1.0, 100.0);

    for (int i = 0; i < count; ++i) {
        std::string sender = users[userDist(gen)];
        std::string receiver;
        do {
            receiver = users[userDist(gen)];
        } while (receiver == sender); // Ensure sender and receiver are different

        double amount = std::round(amountDist(gen) * 100.0) / 100.0; // Round to 2 decimal places

        try {
            Transaction tx(sender, receiver, amount);
            tx.setTxHash();
            transactions.push_back(tx);
        }
        catch (const std::exception& e) {
            std::cerr << "Error creating transaction: " << e.what() << std::endl;
            // Continue with the next iteration if there's an error
        }
    
    }

}

int main() {
    //populate transactions.txt
	populateTransactions(10);

    //Chain attributes: difficulty, block size (threshold)
    Chain blockchain(2,3);

    return 0;
}
