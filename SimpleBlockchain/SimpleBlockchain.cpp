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

int main() {
    Chain blockchain(3);

    // Read transactions from file and add them to the pool
    std::vector<Transaction> txs = readTxsFromFile();
    txs.push_back(readTxsFromFile()[0]);
	cout << txs[0].getSender() << endl;
    for (const auto& tx : txs) {
        blockchain.addTransaction(tx);
        // Sleep for a short time to simulate transactions coming in over time
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Wait for a moment to allow mining to complete
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Display the entire blockchain
    blockchain.displayChain();

    // Verify the blockchain
    bool isChainValid = blockchain.verifyChain();
    std::cout << "Blockchain Verification: " << (isChainValid ? "Valid" : "Invalid") << "\n";

    return 0;
}
