#include <iostream>
#include <string>
#include <vector>
#include "Transaction.h"
#include "Hash.h"
#include "Mine.h"
#include "Block.h"
#include "Utilities.h"
#include <fstream>
#include <iostream>
#include <random>

#include "Chain.h"

using namespace std;

void writeTxsToFile(vector<Transaction> &txs) {
	ofstream output("transactions.txt");
    if (output.is_open()) {
        for (Transaction t : txs) {
			string line = to_string(t.getTime()) + "," + t.getSender() + "," + t.getReceiver() + "," + to_string(t.getAmount()) + "," + t.getTxHash();
            output << line << '\n';
        }
    }
    else {
        cerr << "Error: Could not open transactions.txt for writing." << endl;

    }
    output.close();
}

vector<Transaction> readTxsFromFile() {
    vector<Transaction> txs; 
    ifstream input("transactions.txt");
    if (input.is_open()) {
        string line = "";
        while (getline(input, line)) {
            cout << line << endl;
			vector<string> tokens = helpers::lineSplit(line, ',');
			Transaction temp(tokens[1], tokens[2], stod(tokens[3]), stoi(tokens[0]));
            temp.setTxHash();
			txs.push_back(temp);
        }
    }
    else {
        cerr << "Error: Could not open transactions.txt for reading." << endl;

    }
    return txs;
}

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

	writeTxsToFile(transactions);

}

int main() {
    //populate transactions.txt
	populateTransactions(2);

    //Chain attributes: difficulty, block size
    Chain blockchain(3,1);


    // Read transactions from file and add them to the pool
    std::vector<Transaction> txs = readTxsFromFile();
    txs.push_back(readTxsFromFile()[0]);
    for (const auto& tx : txs) {
        blockchain.addTransaction(tx);
        // Sleep for a short time to simulate transactions coming in over time
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Wait for a moment to allow mining to complete
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Display the entire blockchain
    blockchain.displayChain();

    // Verify the blockchain
    bool isChainValid = blockchain.verifyChain();
    std::cout << "Blockchain Verification: " << (isChainValid ? "Valid" : "Invalid") << "\n";


    blockchain.modifyBlock(1);
    blockchain.displayChain();
    bool isChainValidAfterMod = blockchain.verifyChain();
    std::cout << "Blockchain Verification: " << (isChainValidAfterMod ? "Valid" : "Invalid") << "\n";



    return 0;
}
