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


using namespace std;

void writeTxsToFile(vector<Transaction> &txs) {
	ofstream output("transactions.txt");
    if (output.is_open()) {
        for (Transaction t : txs) {
			string line = to_string(t.getTime()) + "," + t.getSender() + "," + t.getReceiver() + "," + to_string(t.getAmount()) + "," + t.getTxHash();
            output << line << '\n';
        }
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
    return txs;
}

int main()
{
    vector<Transaction> txs = readTxsFromFile();
    string prevHash = "000000";
    unsigned int difficulty = 3;

    Block block(1, prevHash, txs, difficulty);
    block.displayBlock();

    block.mineBlock();
    block.displayBlock();

    bool isMerkleRootValid = block.verifyMerkleRoot(txs);
    cout << "Merkle Root Verification: " << (isMerkleRootValid ? "Valid" : "Invalid") << "\n";

    bool isBlockValid = block.verifyBlock();
    cout << "Block Verification: " << (isBlockValid ? "Valid" : "Invalid") << "\n";

    txs[0] = Transaction("Eve", "Mallory", 50.0);
    txs[0].setTxHash();

    bool isBlockValidAfterTampering = block.verifyMerkleRoot(txs);
    cout << "Merkle Root Verification After Tampering: " << (isBlockValidAfterTampering ? "Valid" : "Invalid") << "\n";

    cout << "Block Hash: " << block.getHash() << "\n";
    cout << "Nonce: " << block.getNonce() << "\n";

    return 0;
}
