#pragma once
#include <ctime>
#include "Address.h"
#include <openssl/sha.h>


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

	time_t getTime() {
		return timestamp;
	}
		
	std::string getSender() {
		return this->sender.address;

	}

	std::string getReceiver() {
		return this->receiver.address;
	}

	double getAmount() {
		return this->amount;
	}

	void setTxHash() {

	}


};

