#pragma once

#include <string>
#include <vector>
#include "Transaction.h"

class Block
{
private:
	unsigned int blockId;
	std::string prevHash;
	std::string hash;
	/*std::string merkleRoot;*/
	std::vector<Transaction> transactions;
	unsigned int nonce;

public:

	bool verifyBlock() {
		std::string data = prevHash + std::to_string(nonce) + transactions[0].getTxHash();
		return verifyHash(data, hash);
	}



};