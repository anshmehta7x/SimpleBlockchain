#pragma once

#include <array>
#include <string>
#include <ctime>
#include "Address.h"
#include "Hash.h"

class Transaction
{	
	
private :
	time_t timestamp;
	Address sender;
	Address receiver;
	double amount;
	std::string txHash;
	
public:

	Transaction() {
		this->timestamp = time(0);
		this->sender = Address();
		this->receiver = Address();
		this->amount = 0.0;
	}

	Transaction(std::string sender, std::string receiver, double amt) {
		if (amt <= 0) {
			throw "Invalid amount";
		}
		time(&timestamp);
		this->sender = Address(sender);
		this->receiver = Address(receiver);
		this->amount = amt;

	}

	time_t getTime() const {
		return timestamp;
	}
		
	 std::string getSender() const {
		return this->sender.address;

	}

	std::string getReceiver() const {
		return this->receiver.address;
	}

	double getAmount() const{
		return this->amount;
	}

	void setTxHash() {
		//combine sender, receiver, timestamp, and amount to create a hash
		std::array <std::string, 4> dataToHash = {
			sender.address,
			receiver.address,
			std::to_string(timestamp),
			std::to_string(amount)
		};

		std::string hashResult = sha256_transaction(dataToHash);
		this->txHash = hashResult;
	}

	std::string getTxHash() const {
		return this->txHash;
	}

};

