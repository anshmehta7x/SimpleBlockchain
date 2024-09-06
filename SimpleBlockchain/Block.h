#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include "Transaction.h"
#include "Mine.h"

class Block
{
private:
	unsigned int difficulty;
	unsigned int blockId;
	std::string prevHash;
	std::string blockHash = "";
	std::string merkleRoot;
	std::vector<Transaction> transactions;
	unsigned int nonce;
	time_t timestamp;
	bool minedStatus = false;


public:

	Block();
	Block(unsigned int id, std::string prevHash, std::vector<Transaction> txs, unsigned int difficulty) {
		this->blockId = id;
		this->prevHash = prevHash;
		this->transactions = txs;
		this->difficulty = difficulty;
		this->timestamp = time(0); 
	}

	std::string calculateMerkleRoot(std::vector<Transaction>& txs) {
		if (txs.empty()) {
			merkleRoot = "";
			return "";
		}

		std::vector<std::string> merkleTree;

		for (const Transaction& t : txs) {
			merkleTree.push_back(t.getTxHash());
		}

		while (merkleTree.size() > 1) {
			std::vector<std::string> newLevel;

			for (size_t i = 0; i < merkleTree.size(); i += 2) {
				if (i + 1 == merkleTree.size()) {
					newLevel.push_back(sha256_block(merkleTree[i] + merkleTree[i]));
				}
				else {
					newLevel.push_back(sha256_block(merkleTree[i] + merkleTree[i + 1]));
				}
			}

			merkleTree = newLevel;
		}

		return merkleTree[0];
	}

	bool verifyMerkleRoot(std::vector<Transaction>& txs) {
		std::string testMerkleRoot = calculateMerkleRoot(txs);
		return testMerkleRoot == this->merkleRoot;
	}


	void mineBlock() {
		// mine the block to calculate hash with target difficulty and receive nonce

		// also calculate merkle root (HO GAYA SYSTUM 🔥🔥🔥🔥🔥)
		
		if (minedStatus) {
			return;
		}
		this->merkleRoot = calculateMerkleRoot(this->transactions);
		std::pair<std::string, int> result = proofOfWork(prevHash, this->merkleRoot, this->difficulty, true);
		this->blockHash = result.first;
		this->nonce = result.second;
		minedStatus = true;
	}

	bool verifyBlock() {
		std::string data = prevHash + std::to_string(nonce) + this->merkleRoot;
		return verifyHash(data, blockHash);
	}

	std::string getHash() const {
		return this->blockHash;
	}

	unsigned int getNonce() const {
		return this->nonce;
	}

	void displayBlock() {
		if (blockHash == "") {
			std::cout << "Block not mined yet\n";
		}
		else {
			//create a box around block

			std::cout << "_________________________________________________________________________________________\n";

			std::cout << "Block ID: " << blockId << "\n";
			std::cout << "Previous Hash: " << prevHash << "\n";
			std::cout << "Hash: " << blockHash << "\n";
			std::cout << "Nonce: " << nonce << "\n";
			std::cout << "Timestamp: " << timestamp << "\n";
			std::cout << "Merkle Root: " << merkleRoot << "\n";
			std::cout << "Transactions: \n";
			for (Transaction tx : transactions) {
				std::cout << "-----------------------------------\n";
				std::cout << "Sender: " << tx.getSender() << "\n";
				std::cout << "Receiver: " << tx.getReceiver() << "\n";
				std::cout << "Amount: " << tx.getAmount() << "\n";
				std::cout << "Timestamp: " << tx.getTime() << "\n";
				std::cout << "Transaction Hash: " << tx.getTxHash() << "\n";
				std::cout << "-----------------------------------\n";
			}			
			std::cout << "_________________________________________________________________________________________\n";

		}
	}


};