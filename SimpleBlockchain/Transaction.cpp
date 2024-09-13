#include "Transaction.h"

Transaction::Transaction() {
    this->timestamp = time(0);
    this->sender = Address();
    this->receiver = Address();
    this->amount = 0.0;
}

Transaction::Transaction(std::string sender, std::string receiver, double amt) {
    if (amt < 0) {
        throw "Invalid amount";
    }
    time(&timestamp);
    this->sender = Address(sender);
    this->receiver = Address(receiver);
    this->amount = amt;
}

Transaction::Transaction(std::string sender, std::string receiver, double amt, time_t timestamp) {
    if (amt < 0) {
        throw "Invalid amount";
    }
    this->timestamp = timestamp;
    this->sender = Address(sender);
    this->receiver = Address(receiver);
    this->amount = amt;
}

time_t Transaction::getTime() const {
    return timestamp;
}

std::string Transaction::getSender() const {
    return this->sender.address;
}

std::string Transaction::getReceiver() const {
    return this->receiver.address;
}

double Transaction::getAmount() const {
    return this->amount;
}

void Transaction::setTxHash() {
    //combine sender, receiver, timestamp, and amount to create a hash
    std::array<std::string, 4> dataToHash = {
        sender.address,
        receiver.address,
        std::to_string(timestamp),
        std::to_string(amount)
    };

    std::string hashResult = sha256_transaction(dataToHash);
    this->txHash = hashResult;
}

std::string Transaction::getTxHash() const {
    return this->txHash;
}

void Transaction::randomModification() {
    this->amount = 12312321;
    this->setTxHash();
}
