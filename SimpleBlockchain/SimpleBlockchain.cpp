#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <chrono>
#include <thread>
#include <limits>
#include "Transaction.h"
#include "Hash.h"
#include "Mine.h"
#include "Block.h"
#include "Utilities.h"
#include "Chain.h"

using namespace std;

void displayMenu() {
    cout << "\n=== Blockchain CLI Menu ===\n";
    cout << "1. Add a transaction\n";
    cout << "2. Display the blockchain\n";
    cout << "3. Verify the blockchain\n";
    cout << "4. Write blockchain to file\n";
    cout << "5. Read blockchain from file\n";
    cout << "6. Modify a block (for testing)\n";
    cout << "7. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    cout << "Welcome to the Simple Blockchain CLI!\n";
    int difficulty = 3, blockSize = 5;
	cout << "Enter the difficulty level for the blockchain: \n";
	cin >> difficulty;
	cout << "Enter the block size for the blockchain: \n";
	cin >> blockSize;

    Chain blockchain(difficulty, blockSize);
    string sender, receiver, filename;
    double amount;
    int choice, blockIndex;


    while (true) {
        displayMenu();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                cout << "Enter sender: ";
                getline(cin, sender);
                cout << "Enter receiver: ";
                getline(cin, receiver);
                cout << "Enter amount: ";
                cin >> amount;
                blockchain.addTransaction(Transaction(sender, receiver, amount));
                break;
            case 2:
                cout << "\n=== Current Blockchain ===\n";
                blockchain.displayChain();
                break;
            case 3:
                if (blockchain.verifyChain()) {
                    cout << "Blockchain is valid.\n";
                } else {
                    cout << "Blockchain is invalid!\n";
                }
                break;
            case 4:
                cout << "Enter filename to save the blockchain: ";
                cin >> filename;
                if (blockchain.writeCurrentChainToFile(filename)) {
                    cout << "Blockchain written to file successfully.\n";
                } else {
                    cout << "Failed to write blockchain to file.\n";
                }
                break;
            case 5:
                cout << "Enter filename to read the blockchain from: ";
                cin >> filename;
                if (blockchain.readChainFromFile(filename)) {
                    cout << "Blockchain read from file successfully.\n";
                } else {
                    cout << "Failed to read blockchain from file.\n";
                }
                break;
            case 6:
                cout << "Enter the index of the block to modify: ";
                cin >> blockIndex;
                if (blockIndex >= 0 && blockIndex < blockchain.getChainSize()) {
                    blockchain.modifyBlock(blockIndex);
                    cout << "Block " << blockIndex << " has been modified.\n";
                } else {
                    cout << "Invalid block index.\n";
                }
                break;
            case 7:
                cout << "Exiting the program. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}