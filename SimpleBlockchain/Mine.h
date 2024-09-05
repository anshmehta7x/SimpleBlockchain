#pragma once

#include "Hash.h"
#include <vector>
#include "Transaction.h"
#include <utility>

// set mining time by 'difficulty' value

bool startsWith(const std::string& str, const std::string& prefix) {
	return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}


bool checkIfValidHash(std::string& hash, unsigned int difficulty) {
	// check  if produced hash starts with specified number of 0s (specified by difficulty value)
	std::string target(difficulty, '0');
	return startsWith(hash, target);
}

std::pair<std::string,int> mineBlock(const std::string& prevHash, const std::vector<Transaction>& transactions, unsigned int difficulty, bool verbose = false) {
	int nonce = 0;
	std::string hash;

	do {
		 hash = sha256_block(prevHash + std::to_string(++nonce) + transactions[0].getTxHash());
		 if (verbose) {
			 std::cout << "current hash: " << hash <<" , current nonce:"<< nonce<< std::endl;
		 }
	} while (!checkIfValidHash(hash, difficulty));

	std::pair<std::string, int> result = { hash, nonce };
	return result;
}


