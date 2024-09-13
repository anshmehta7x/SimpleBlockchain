#pragma once

#include "Hash.h"
#include <vector>
#include "Transaction.h"
#include <utility>
#include <string>

// Function declarations
bool startsWith(const std::string& str, const std::string& prefix);

bool checkIfValidHash(std::string& hash, unsigned int difficulty);

std::pair<std::string, int> proofOfWork(const std::string& prevHash, const std::string merkleRoot, unsigned int difficulty, bool verbose = false);
