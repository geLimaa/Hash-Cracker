#pragma once

#include <string>
#include <vector>

std::string dictionaryAttack(const std::string& hash, const std::string& algorithm, const std::string& path);

static void dictionaryAttackWorker(const std::vector<std::string>& words, int start, int end, const std::string& hash, std::string (*hashFunction)(const std::string&), std::string& sharedResult, bool& found);

std::string dictionaryAttackThreaded(const std::string& hash, const std::string& algorithm, const std::string& path, int numThreads);