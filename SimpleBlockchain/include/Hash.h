#pragma once

#include <string>
#include <array>

std::string sha256_transaction(const std::array<std::string, 4>& arr);
std::string sha256_block(const std::string& data);
bool verifyHash(const std::string& data, const std::string& hash);

