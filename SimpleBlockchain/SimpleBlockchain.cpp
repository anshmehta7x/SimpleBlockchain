#include <iostream>
#include "Transaction.h"
#include "Hash.h"
#include "Mine.h"
#include "Block.h"
#include <string>
#include <vector>

int main()
{
    Transaction t1("Alice", "Bob", 10.0);
    t1.setTxHash();
    Transaction t2("Bob", "Charlie", 5.0);
    t2.setTxHash();
    Transaction t3("Charlie", "Alice", 3.0);
    t3.setTxHash();

    std::vector<Transaction> txs = { t1, t2, t3 };

    std::string prevHash = "000000";
    unsigned int difficulty = 3;

    Block block(1, prevHash, txs, difficulty);
    block.displayBlock();

    block.mineBlock();
    block.displayBlock();

    bool isMerkleRootValid = block.verifyMerkleRoot(txs);
    std::cout << "Merkle Root Verification: " << (isMerkleRootValid ? "Valid" : "Invalid") << "\n";

    bool isBlockValid = block.verifyBlock();
    std::cout << "Block Verification: " << (isBlockValid ? "Valid" : "Invalid") << "\n";

    txs[0] = Transaction("Eve", "Mallory", 50.0);
    txs[0].setTxHash();

    bool isBlockValidAfterTampering = block.verifyMerkleRoot(txs);
    std::cout << "Merkle Root Verification After Tampering: " << (isBlockValidAfterTampering ? "Valid" : "Invalid") << "\n";

    std::cout << "Block Hash: " << block.getHash() << "\n";
    std::cout << "Nonce: " << block.getNonce() << "\n";

    return 0;
}
