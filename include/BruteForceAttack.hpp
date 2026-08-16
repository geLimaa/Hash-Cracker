#pragma once 

#include <string>

static std::string bruteForce(const std::string& prefix, int remaining, const std::string& charset, const std::string& hash, std::string (*hashFunction)(const std::string&));

static void bruteForceThreaded(const std::string& prefix, int remaining, const std::string& charset, const std::string& hash, std::string (*hashFunction)(const std::string&), std::string& sharedResult, bool& found);

std::string bruteForceAttack(const std::string& hash, const std::string& algorithm, const std::string& charset, int maxLength);


std::string bruteForceAttackThreaded(const std::string& hash, const std::string& algorithm, const std::string& charset, int maxLength, int numThreads);