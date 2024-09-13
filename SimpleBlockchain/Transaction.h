#pragma once

#include <array>
#include <string>
#include <ctime>
#include "Address.h"
#include "Hash.h"

class Transaction
{
private:
    time_t timestamp;
    Address sender;
    Address receiver;
    double amount;
    std::string txHash;

public:
    Transaction();
    Transaction(std::string sender, std::string receiver, double amt);
    Transaction(std::string sender, std::string receiver, double amt, time_t timestamp);

    time_t getTime() const;
    std::string getSender() const;
    std::string getReceiver() const;
    double getAmount() const;
    void setTxHash();
    std::string getTxHash() const;
    void randomModification();
};
