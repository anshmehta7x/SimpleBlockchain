#include "Hash.h"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

std::string sha256_transaction(const std::array<std::string, 4>& arr) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    std::string input = "";
    for (const std::string& i : arr) {
        input += i;
    }
    SHA256(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), hash);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }

    return ss.str();
}

std::string sha256_block(const std::string& data) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }

    return ss.str();
}

bool verifyHash(const std::string& data, const std::string& hash) {
    return sha256_block(data) == hash;
}
