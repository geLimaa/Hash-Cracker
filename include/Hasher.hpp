#pragma once

#include <string>
#include <openssl/evp.h>

static std::string computeHash(const std::string& input, const EVP_MD* algorithm);

std::string md5(const std::string& input);
std::string sha1(const std::string& input);
std::string sha256(const std::string& input);
std::string sha512(const std::string& input);

std::string detectHashType(const std::string& hash);
