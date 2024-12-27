#pragma once
#include <string>
#include <regex>

class Address {
public:
	std::string address;

	Address() {
		this->address = "0x1111111111";
	}

	Address(std::string addr) {
		if (validateAddress(addr)) {
			this->address = addr;
		}
	}

	bool validateAddress(std::string addr) {
		//add support for this later
		return true;
	}
	
};