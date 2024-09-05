#pragma once

#include <openssl/sha.h>
#include <string>
#include <sstream>
#include <array>
#include <iomanip>


std::string sha256(std::array<std::string,4>& arr) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    std::string input = "";
    for (std::string& i : arr) {
        input += i;
    }
    SHA256(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), hash);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }

    return ss.str();
}
